# Cub3D - 3D Raycasting Game Engine

## 概要
Cub3Dは、レイキャスティング技術を使用した3Dゲームエンジンです。  
42TokyoのプロジェクトとしてC言語とMiniLibXライブラリを使用して実装されています。

## 機能

### ✅ 実装済み機能
- **3Dレイキャスティング描画** - リアルタイム3D環境表示
- **テクスチャマッピング** - 壁面へのテクスチャ適用（4方向対応）
- **プレイヤー移動・回転** - WASD移動、矢印キー回転
- **ミニマップ表示** - リアルタイムマップ表示（左上角）
- **設定ファイル解析** - .cubファイルによるマップ・設定読み込み
- **マップ検証** - 境界・閉塞性・プレイヤー位置の検証
- **フレームレート制限** - 60FPS制限によるパフォーマンス最適化
- **エラーハンドリング** - 安全なリソース管理

### 🎮 操作方法
- **W, A, S, D** : 前進・左移動・後退・右移動
- **←, →** : 左回転・右回転
- **ESC** : ゲーム終了

### 📁 ファイル構成
```
cub3d/
├── include/
│   ├── cub3d.h          # 関数プロトタイプ
│   ├── structures.h     # データ構造定義
│   └── constants.h      # 定数定義
├── src/
│   ├── main.c           # メイン処理
│   ├── init.c           # 初期化処理
│   ├── parser.c         # 設定ファイル解析
│   ├── map_parser.c     # マップデータ解析
│   ├── map_validation.c # マップ検証
│   ├── raycasting.c     # レイキャスティング
│   ├── texture.c        # テクスチャ処理
│   ├── render.c         # 描画処理
│   ├── minimap.c        # ミニマップ描画
│   ├── events.c         # イベント処理
│   ├── cleanup.c        # リソース解放
│   ├── error.c          # エラー処理
│   ├── utils.c          # ユーティリティ関数
│   └── file_utils.c     # ファイル操作
├── assets/
│   └── test.cub         # テスト用マップファイル
└── Makefile             # ビルド設定
```

## ビルド・実行方法

### 必要環境
- Linux (X11対応)
- GCC コンパイラ
- Make
- MiniLibX (自動ダウンロード・セットアップ)

### ビルド
```bash
make
```

### 実行
```bash
./cub3D assets/test.cub
```

### クリーンアップ
```bash
make clean    # オブジェクトファイル削除
make fclean   # 実行ファイル削除
make re       # 再ビルド
```

## 設定ファイル（.cub）形式
```
NO ./assets/north_texture.xpm
SO ./assets/south_texture.xpm
WE ./assets/west_texture.xpm
EA ./assets/east_texture.xpm

F 220,100,0
C 225,30,0

111111111
100000001
100N00001
100000001
111111111
```

## 技術仕様
- **言語**: C (C99標準)
- **グラフィクス**: MiniLibX
- **レンダリング**: レイキャスティング
- **フレームレート**: 60FPS制限
- **画面解像度**: 1024x768
- **ミニマップ**: 200x200 (左上角)

## 作者
42Tokyo Student - cub3D Project Implementation

---
*本プロジェクトは42Tokyoの課題として実装されました。*
