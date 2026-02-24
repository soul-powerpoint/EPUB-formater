from PIL import Image
import sys

def cover_info(path: str):
    img = Image.open(path)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: cover_info.py <image_path>", file=sys.stderr)
        sys.exit(2)
    cover_info(sys.argv[1])
