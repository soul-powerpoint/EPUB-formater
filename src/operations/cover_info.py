from src.epub_io.opf import ManifestItem
from PIL import Image
import sys
import os

def cover_info(path: str):
    img = Image.open(path)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)


def cover_info_epub(epub_path: str, opf_path: str, cover_item: ManifestItem) -> None:
    cover_zip_path = cover_item.href
    opf_dir = os.path.dirname(opf_path)
    full_path = os.path.join(opf_dir, cover_zip_path) if opf_dir else cover_zip_path

    img = Image.open(full_path)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)
    return None


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: cover_info.py <image_path>", file=sys.stderr)
        sys.exit(2)
    cover_info(sys.argv[1])
