#!/usr/bin/env python3
"""把手机原图压成适合 GitHub 网页展示的尺寸，顺手清掉 EXIF。

用法：
    1. 把原图丢进 docs/images/_raw/
    2. python tools/compress_images.py
    3. 输出在 docs/images/，文件名已规范化

会做三件事：缩到长边 1280、转成 JPG 质量 82、抹掉 EXIF（含 GPS 定位）。
PNG 截图默认保留 PNG（文字更清晰），只做尺寸与体积控制。
"""

from __future__ import annotations

import sys
from pathlib import Path

try:
    from PIL import Image, ImageOps
except ImportError:
    sys.exit("缺少 Pillow，先执行：pip install pillow")

MAX_EDGE = 1280
JPEG_QUALITY = 82
MAX_BYTES = 500 * 1024

ROOT = Path(__file__).resolve().parent.parent
RAW = ROOT / "docs" / "images" / "_raw"
OUT = ROOT / "docs" / "images"


def slugify(name: str) -> str:
    """中文文件名没法自动翻译，保留字母数字，其余折成连字符。"""
    keep = [c.lower() if c.isalnum() and c.isascii() else "-" for c in name]
    slug = "".join(keep)
    while "--" in slug:
        slug = slug.replace("--", "-")
    return slug.strip("-") or "image"


def convert(path: Path) -> Path | None:
    try:
        with Image.open(path) as im:
            im = ImageOps.exif_transpose(im)  # 按 EXIF 方向摆正，再抹掉 EXIF
            is_png = path.suffix.lower() == ".png"

            if max(im.size) > MAX_EDGE:
                scale = MAX_EDGE / max(im.size)
                im = im.resize(
                    (round(im.width * scale), round(im.height * scale)),
                    Image.LANCZOS,
                )

            if is_png:
                target = OUT / f"{slugify(path.stem)}.png"
                im.save(target, "PNG", optimize=True)
            else:
                target = OUT / f"{slugify(path.stem)}.jpg"
                if im.mode not in ("RGB", "L"):
                    im = im.convert("RGB")

                quality = JPEG_QUALITY
                while quality >= 50:
                    im.save(target, "JPEG", quality=quality, optimize=True, progressive=True)
                    if target.stat().st_size <= MAX_BYTES:
                        break
                    quality -= 8
                else:
                    im.save(target, "JPEG", quality=50, optimize=True, progressive=True)

            size_kb = target.stat().st_size / 1024
            flag = "" if size_kb <= 500 else "  ← 仍超 500KB，考虑裁掉无关区域"
            print(f"{path.name}  ->  {target.name}  {im.width}x{im.height}  {size_kb:.0f} KB{flag}")
            return target
    except Exception as exc:  # 单张失败不影响其他
        print(f"{path.name}  跳过：{exc}")
        return None


def main() -> int:
    RAW.mkdir(parents=True, exist_ok=True)
    OUT.mkdir(parents=True, exist_ok=True)

    patterns = ("*.jpg", "*.jpeg", "*.png", "*.webp", "*.bmp", "*.JPG", "*.JPEG", "*.PNG")
    files = sorted({p for pat in patterns for p in RAW.glob(pat)})
    if not files:
        print(f"把原图放进 {RAW} 再运行。")
        return 0

    ok = sum(1 for f in files if convert(f))
    print(f"\n完成 {ok}/{len(files)} 张，输出目录：{OUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
