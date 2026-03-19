import zipfile
import os
import io
from PIL import Image
from .opf import ManifestItem


def read_cover_image(epub_path: str, opf_path: str, cover_item: ManifestItem) -> Image.Image:
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

    return Image.open(io.BytesIO(image_bytes))
