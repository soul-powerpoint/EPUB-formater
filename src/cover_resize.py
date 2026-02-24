from PIL import Image
import sys

def cover_resize_stretch(img: Image.Image, ratio: float) -> Image.Image:
    width, height = img.size
    new_h = int(round(width * ratio))
    new_w = width

    out = img.resize((new_w, new_h), Image.BICUBIC)
    out.show()
    return out

# TODO
def cover_resize_crop(img: Image.Image, target_w: int, target_h: int, *, center = (0.5, 0.5)) -> Image.Image:
    width, height = img.size
    scale = max(target_w / width, target_h / height)
    new_w, new_h = int(round(width * scale)), int(round(height * scale))

    img = img.resize((new_w, new_h), Image.Resampling.LANCZOS)

    cx, cy = center
    left = int(round((new_w - target_w) * cx))
    top  = int(round((new_h - target_h) * cy))

    left = max(0, min(left, new_w - target_w))
    top  = max(0, min(top,  new_h - target_h))

    return img.crop((left, top, left + target_w, top + target_h))


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: cover_resize.py <image_path>", file=sys.stderr)
        sys.exit(2)
    inp = sys.argv[1]
    out = "output.jpg"

    target_w, target_h = 100, 100
    ratio = 1.5

    img = Image.open(inp)
    img = img.convert("RGB") if img.mode in ("P", "RGBA", "LA") else img
    # out_im = cover_resize_crop(img, target_w, target_h, center=(0.5, 0.5))
    out_im = cover_resize_stretch(img, ratio)
    out_im.save(out, quality=95)
