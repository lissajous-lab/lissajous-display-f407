# 照片与视频怎么放

一句话：**照片进仓库（压过再进），视频不进仓库（走 Release 附件 + B 站）。**

---

## 1. 为什么视频不能进 Git

Git 保存的是每次提交的完整快照。一个 200 MB 的视频提交进去，仓库永久多 200 MB，
以后每个人 `git clone` 都要下这 200 MB，而且**删不掉**——`git rm` 只是新增一次删除提交，
历史里那份二进制还在。

GitHub 还有硬限制：

| 限制 | 数值 |
|---|---|
| 单个文件 | 100 MB 以上直接拒绝推送 |
| 仓库建议体积 | 1 GB 以下 |
| 单次 push | 2 GB |

所以视频的归宿只有两个：**Release 附件**（不占仓库体积）或**外部平台**（B 站 / 网盘）。

---

## 2. 视频：首选 B 站 + Release 双保险

### 2.1 传 B 站（能被别人看到）

1. 上传到 B 站，标题写清楚：`2026 电赛 F 题 李萨如图形显示控制装置 完整演示`
2. 简介里挂仓库链接：`https://github.com/13788646421ab-sys/lissajous-display-f407`
3. 拿到 BV 号，填进 README

GitHub 的 README **不渲染 `<iframe>`**（会剥掉），所以只能用「缩略图 + 链接」的写法：

```markdown
[![演示视频](docs/images/video-thumb.jpg)](https://www.bilibili.com/video/BV1xxxxxxxxx)
```

缩略图就是视频封面截图，压到 500 KB 以内放进 `docs/images/`。
点图跳到 B 站，这是 GitHub 上的通行做法。

### 2.2 传 Release（保证永不丢失）

B 站视频可能被吞、被限流，Release 附件是仓库自带的存档：

1. 仓库首页右侧 **Releases → Draft a new release**
2. Tag 填 `v1.0`，标题 `赛后整理版`
3. 把 `demo-full.mp4` 和 `FFT.hex` 一起拖进附件区
4. 发布

单文件上限 2 GB，够用。附件不计入仓库体积，clone 时也不会下载。

### 2.3 视频多大算合适

| 内容 | 建议 |
|---|---|
| 完整演示（一条走完六道题） | 2~4 分钟，1080p，**50 MB 以内** |
| 单个功能片段（锁相、切档、视觉识别） | 15~30 秒，各 5 MB 以内 |

手机横屏拍就行，别开 4K——没有意义，只会变大。

---

## 3. 照片：进仓库，但必须处理

### 3.1 三条硬要求

| 项 | 要求 | 原因 |
|---|---|---|
| 体积 | 单张 ≤ 500 KB | 仓库要轻 |
| 长边 | ≤ 1280 px | GitHub 正文栏约 800 px 宽，更大是浪费 |
| EXIF | 必须清掉 | **手机照片带 GPS 坐标，公开仓库等于公开你家地址** |

第三条最容易被忽略，也最要紧。手机拍的图默认写入拍摄地点，
GitHub 上任何人下载原图就能读到经纬度。

### 3.2 操作方法

```bash
# 1. 原图丢进这个目录（已被 .gitignore 排除，不会误提交）
docs/images/_raw/

# 2. 一键压缩 + 清 EXIF + 规范命名
python tools/compress_images.py

# 3. 输出在 docs/images/，确认效果后 _raw/ 可删
```

脚本做了三件事：按 EXIF 方向摆正、缩到长边 1280、抹掉全部元数据。
JPG 会边压边试探，直到体积进 500 KB 以内。

### 3.3 建议拍哪些

见 [`docs/images/README.md`](images/README.md)，重点是这五张：

| 文件名 | 内容 |
|---|---|
| `board-overview.jpg` | 整机俯视，各模块位置标清楚 |
| `analog-frontend.jpg` | 输入/输出调理的洞洞板 |
| `scope-locked.jpg` | 示波器上静止的李萨如图形 |
| `scope-infinity.jpg` | 二倍频的「∞」形 |
| `k230-setup.jpg` | K230 摄像头对准屏幕的布置 |

拍的时候把示波器面板挡住，只留屏幕——这也是赛题本身的要求。

---

## 4. 引用写法

在 README 或 `docs/hardware.md` 里：

```markdown
![整机硬件组成](docs/images/board-overview.jpg)
```

正文里的图**不要**写 `<img width="600">`，GitHub 会按栏宽自适应，硬写宽度在手机上会溢出。

多图并排用 HTML 表格，比连续堆图好看：

```markdown
| 锁定后的圆 | 二倍频 ∞ 形 |
|---|---|
| ![](docs/images/scope-locked.jpg) | ![](docs/images/scope-infinity.jpg) |
```

---

## 5. 别做的事

- ❌ 把 mp4 直接 `git add` 进仓库
- ❌ 用 Git LFS 存视频（免费额度 1 GB 存储，clone 时还要额外拉，对看代码的人是负担）
- ❌ 传未压缩的手机原图（5 MB 一张，十张就把仓库撑到 50 MB）
- ❌ 传厂商 datasheet PDF（版权问题，改成官方链接）
- ❌ 传参赛论文、获奖证书扫描件（含个人信息，且不是代码的一部分）
