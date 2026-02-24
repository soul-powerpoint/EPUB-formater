from PIL import Image
import sys

def cover_ratio(img: Image) -> float:
    w, h = img.size
    ratio = h / w
    return ratio


def get_side(side: int, new_side: int):
    start_side = round((side - new_side) / 2, 0)
    end_side = start_side + new_side
    return start_side, end_side


def cover_resize_stretch(img: Image.Image, ratio: float) -> Image.Image:
    width, height = img.size
    new_h = int(round(width * ratio))
    new_w = width

    out = img.resize((new_w, new_h), Image.Resampling.LANCZOS)
    out.show()
    return out


def cover_resize_crop(img: Image.Image, ratio: float) -> Image.Image:
    current_ratio = cover_ratio(img)
    width, height = img.size

    if current_ratio == ratio:
        return img

    if current_ratio > ratio:
        base = width
        side = height
        new_side = base * ratio
        start_side, end_side = get_side(side, new_side)
        return img.crop((0, start_side, base, end_side))
    elif current_ratio < ratio:
        base = height
        side = width
        new_side = base / ratio
        start_side, end_side = get_side(side, new_side)
        return img.crop((start_side, 0, end_side, base))


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("usage: cover_resize.py <image_path> <ratio>", file=sys.stderr)
        sys.exit(2)
    in_path = sys.argv[1]
    out_path = "output.jpg"

    target_w, target_h = 100, 100
    ratio = sys.argv[2]

    img = Image.open(in_path)
    img = img.convert("RGB") if img.mode in ("P", "RGBA", "LA") else img

    # out_img = cover_resize_crop(img, ratio)
    out_img = cover_resize_stretch(img, ratio)
    out_img.save(out_path, quality=95)
