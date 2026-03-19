from src.epub_io.opf import ManifestItem
from PIL import Image
import sys
import os
import io
import zipfile

def cover_info(path: str):
    img = Image.open(path)
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)


def cover_info_epub(epub_path: str, opf_path: str, cover_item: ManifestItem) -> float:
    cover_zip_path = cover_item.href
    opf_dir = os.path.dirname(opf_path)
    full_path = os.path.join(opf_dir, cover_zip_path) if opf_dir else cover_zip_path

    with zipfile.ZipFile(epub_path, "r") as epub:
        names = epub.namelist()
        if full_path in names:
            image_bytes = epub.read(full_path)
        else:
            match = [n for n in names if n.endswith(cover_zip_path)]
            if not match:
                raise FileNotFoundError(f"Cover '{cover_zip_path}' not found in EPUB")
            image_bytes = epub.read(match[0])

    img = Image.open(io.BytesIO(image_bytes))
    w, h = img.size
    ratio = h / w

    print("width:", w, "height:", h, "ratio:", ratio)
    return ratio


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: cover_info.py <image_path>", file=sys.stderr)
        sys.exit(2)
    cover_info(sys.argv[1])
