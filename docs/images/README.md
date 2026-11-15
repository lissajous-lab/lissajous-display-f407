# 图片素材

本目录放 README 与 `docs/hardware.md` 引用的照片。

## 规则

| 项 | 要求 | 原因 |
|---|---|---|
| 单张体积 | **≤ 500 KB** | 仓库是纯文本主导的，图太大 clone 会变慢 |
| 长边 | ≤ 1280 px | GitHub 正文栏宽约 800 px，再大是浪费 |
| 格式 | JPG（照片）/ PNG（截图带文字） | 照片用 JPG 可压到 1/10 |
| 文件名 | 小写英文 + 连字符，**不要中文和空格** | 中文名在部分环境会 404 |
| EXIF | 必须清除 | 手机照片带 GPS 定位，公开仓库等于公开住址 |

## 建议拍的内容

| 文件名 | 内容 | 状态 |
|---|---|---|
| `board-overview.jpg` | 整机俯视，能看出各模块怎么拼起来 | 已收录 |
| `bench-setup.jpg` | 桌面布置：信号源、示波器、笔记本、装置 | 已收录 |
| `k230-detection.jpg` | K230 相机画面与识别框（∞ 形） | 已收录 |
| `analog-frontend.jpg` | 输入/输出调理板的正反面特写（没有原理图，照片就是唯一资料） | 待拍 |
| `scope-locked.jpg` | 示波器上静止的李萨如图形 | 待拍 |
| `scope-infinity.jpg` | 示波器上真实的「∞」形 | 待拍 |
| `k230-setup.jpg` | K230 摄像头对准示波器屏幕的机械布置 | 待拍 |

`k230-detection.jpg` 是软件截图，本身只有 728 px 宽，无需再压缩。

另有一张 `video-cover.jpg`（1280×720，158 KB）：B 站视频封面，只用作 README 里跳转视频的缩略图，不属于拍摄清单。

## 怎么处理

把手机原图放进 `docs/images/_raw/`（该目录已被 `.gitignore` 排除，不会被提交），然后：

```bash
python tools/compress_images.py
```

脚本会压缩、改名为规范文件名、清掉 EXIF，输出到 `docs/images/`。
确认效果后用 `_raw/` 里的原图可以删掉。

## 引用写法

```markdown
![整机照片](docs/images/board-overview.jpg)
```

正文里的图不要加 `width` 属性，GitHub 会按栏宽自适应。

## 视频不放这里

演示视频不放进仓库，见根目录 `README.md` 的「演示视频」一节说明。
