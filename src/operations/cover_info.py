from PIL import Image
import sys
import os
import io
import zipfile
from src.epub_io.opf import ManifestItem
from src.epub_io.cover import read_cover_image

def cover_info(path: str):
    img = Image.open(path)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)


def cover_info_epub(epub_path: str, opf_path: str, cover_item: ManifestItem) -> float:
    img = read_cover_image(epub_path, opf_path, cover_item)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)
    return ratio


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: cover_info.py <image_path>", file=sys.stderr)
        sys.exit(2)
    cover_info(sys.argv[1])
