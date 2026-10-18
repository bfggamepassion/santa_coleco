"""Shared helpers: read const byte arrays from charset.c, Coleco RLE, TMS9918 palette."""
import re

PAL = [(0,0,0),(0,0,0),(33,200,66),(94,220,120),(84,85,237),(125,118,252),
       (212,82,77),(66,235,245),(252,85,84),(255,121,120),(212,193,84),
       (230,206,128),(33,176,59),(201,91,186),(204,204,204),(255,255,255)]

def read_arrays(path):
    src = open(path, encoding='latin-1').read()
    out = {}
    for m in re.finditer(r'const byte\*?\s+(\w+)\[\]\s*=\s*\{(.*?)\};', src, re.S):
        vals = re.findall(r'0x[0-9A-Fa-f]+|\b\d+\b|\w+', m.group(2))
        try:
            out[m.group(1)] = [int(v, 0) for v in vals]
        except ValueError:
            pass  # pointer tables
    return out

def rle_decode(d):
    o = []; i = 0
    while d[i] != 0xFF:
        n = d[i]; i += 1
        if n & 0x80:
            o += [d[i]] * ((n & 0x7F) + 1); i += 1
        else:
            o += d[i:i + n + 1]; i += n + 1
    return o

def rle_encode(data):
    """Format of rle2vram: 0x80|(n-1) + byte = run of n (3..127), (n-1) + n bytes = literal (1..128), 0xFF = end."""
    out = []; lit = []; i = 0
    def flush():
        while lit:
            chunk = lit[:128]; del lit[:128]
            out.append(len(chunk) - 1); out.extend(chunk)
    while i < len(data):
        j = i
        while j < len(data) and data[j] == data[i] and j - i < 127:
            j += 1
        if j - i >= 3:
            flush(); out += [0x80 | (j - i - 1), data[i]]; i = j
        else:
            lit.append(data[i]); i += 1
            if len(lit) == 128: flush()
    flush(); out.append(0xFF)
    assert rle_decode(out) == list(data)
    return out

def c_array(name, data, per_line=16):
    lines = []
    for k in range(0, len(data), per_line):
        lines.append('  ' + ', '.join('0x%02X' % b for b in data[k:k + per_line]))
    return 'const byte %s[] = {\n%s};\n' % (name, ',\n'.join(lines))

def nearest(rgb):
    return min(range(1, 16), key=lambda c: sum((a - b) ** 2 for a, b in zip(PAL[c], rgb)))

def encode_row(px):
    """px: 8 TMS colour codes -> (pattern byte, colour byte). Resolves >2 colours by least RGB error."""
    cols = sorted(set(px), key=lambda c: (-px.count(c), c))
    if len(cols) == 1:
        return 0x00, cols[0]                      # all background
    if len(cols) > 2:
        best = None
        for a in cols:
            for b in cols:
                if a >= b: continue
                err = sum(min(sum((x - y) ** 2 for x, y in zip(PAL[p], PAL[a])),
                              sum((x - y) ** 2 for x, y in zip(PAL[p], PAL[b]))) for p in px)
                if best is None or err < best[0]: best = (err, a, b)
        _, a, b = best
        dist = lambda p, q: sum((x - y) ** 2 for x, y in zip(PAL[p], PAL[q]))
        px = [p if p in (a, b) else (a if dist(p, a) <= dist(p, b) else b) for p in px]
        cols = sorted(set(px), key=lambda c: (-px.count(c), c))
    bg, fg = cols[0], cols[1]
    pat = 0
    for k, p in enumerate(px):
        if p == fg: pat |= 0x80 >> k
    return pat, (fg << 4) | bg
