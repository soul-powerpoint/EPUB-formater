# EPUB-formater
A CLI tool for editing EPUB metadata and cover images.


## Installation
 
```bash
git clone https://github.com/your-username/EPUB-formater.git
cd EPUB-formater
pip install -e .
```

## Usage

```
epub-formater [OPTIONS] <epub>
```


### Options

| Option | Description |
|--------|-------------|
| `-info` | Show metadata of the EPUB file |
| `-set-language LANG` | Set the language (BCP-47 code) |
| `-set-cover IMAGE` | Replace the cover image |
| `-download-cover` | Download the cover image to current directory |
| `-cover-info` | Show cover image dimensions and ratio |
| `-resize-cover {stretch,crop}` | Resize cover using the specified method |
| `-ratio RATIO` | Target height/width ratio, used with `-resize-cover` (default: 1.5) |


## Examples

### View EPUB metadata

```bash
epub-formater -info ./books/my_book.epub
```

Output:

```
Title:    World So Wide
Author:   Sinclair Lewis
Language: en
Cover ID: cover-image
```

### Set language

```bash
epub-formater -set-language ja ./books/my_book.epub
```

### Replace cover image

```bash
epub-formater -set-cover ./new_cover.jpg ./books/my_book.epub
```

### Download cover

```bash
epub-formater -download-cover ./books/my_book.epub
```

Saves the cover image to the current directory with its original filename.

### View cover info

```bash
epub-formater -cover-info ./books/my_book.epub
```

Output:

```
width: 800 height: 1200 ratio: 1.5
```

### Resize cover

```bash
# Stretch to a 1.6 ratio
epub-formater -resize-cover stretch -ratio 1.6 ./books/my_book.epub

# Crop to a 1.5 ratio (default)
epub-formater -resize-cover crop ./books/my_book.epub
```


## Requirements

- Python >= 3.10
- Pillow
