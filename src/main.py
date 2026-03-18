import argparse
import sys
from .formator import EpubFormator

def command_info(args):
    fmt = EpubFormator(args.epub)
    print(f"Title: \t\t {fmt.get_title()}")
    print(f"Author: \t {fmt.get_author()}")
    print(f"Language: \t {fmt.get_language()}")
    print(f"Cover ID: \t {fmt.get_cover()}")


def command_set_language(args):
    fmt = EpubFormator(args.epub)
    fmt.set_language(args.language)
    print(f"Language set to {args.language}")


def command_set_cover(args):
    fmt = EpubFormator(args.epub)
    fmt.set_cover(args.image)
    print(f"Cover replaced with {args.image}")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog = "epub-formater",
        description = "A CLI tool for editing EPUB metadata"
    )
    sub = parser.add_subparsers(dest = "command", required = True)

    p_info = sub.add_parser("info", help = "Show metadata of the EPUB file")
    p_info.add_argument("epub", help="Path to the EPUB file")
    p_info.set_defaults(func = command_info)

    p_lang = sub.add_parser("set-language", help="Set the language of the EPUB file")
    p_lang.add_argument("epub", help="Path to the EPUB file")
    p_lang.add_argument("language", help="BCP-47 language code, e.g. 'en', 'ja', 'zh-CN'")
    p_lang.set_defaults(func = command_set_language)

    p_cover = sub.add_parser("set-cover", help="Replace the cover image of an EPUB file")
    p_cover.add_argument("epub", help="Path to the EPUB file")
    p_cover.add_argument("image", help="Path to the new cover image")
    p_cover.set_defaults(func = command_set_cover)

    return parser


def main():
    parser = build_parser()
    args = parser.parse_args()

    try:
        args.func(args)
    except FileNotFoundError as e:
        print(f"Error: {e}", file = sys.stderr)
        sys.exit(1)
    except ValueError as e:
        print(f"Error: {e}", file = sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Unexpected error: {e}", file = sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
