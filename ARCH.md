```
cub3D/
│
├── Makefile                 # コンパイル設定
├── includes/                # ヘッダファイル
│   ├── cub3d.h              # メインヘッダ
│   ├── structures.h         # データ構造の定義
│   └── constants.h          # 定数の定義
│
├── src/                     # ソースファイル
│   ├── main.c               # メインエントリポイント
│   ├── init/                # 初期化関連
│   │   ├── init_data.c      # データ初期化
│   │   ├── validate.c       # 入力検証
│   │   └── init_mlx.c       # MiniLibX初期化
│   │
│   ├── parser/              # パーサー関連
│   │   ├── get_file_contents.c    # ファイル読み込み
│   │   ├── set_data_from_file.c   # データ設定
│   │   └── parse_map.c      # マップ解析
│   │
│   ├── map/                 # マップ処理関連
│   │   ├── check_map.c      # マップ検証
│   │   ├── check_components.c     # 要素検証
│   │   ├── check_player.c   # プレイヤー検証
│   │   └── flood_fill.c     # マップの閉鎖性検証
│   │
│   ├── rendering/           # 描画関連
│   │   ├── rendering.c      # メイン描画処理
│   │   ├── raycasting.c     # レイキャスティング
│   │   ├── draw_walls.c     # 壁描画
│   │   ├── draw_floor_ceiling.c   # 床・天井描画
│   │   └── texture_utils.c  # テクスチャ関連ユーティリティ
│   │
│   ├── player/              # プレイヤー関連
│   │   ├── handle_movement.c      # 移動処理
│   │   ├── handle_rotation.c      # 回転処理
│   │   └── collision.c      # 衝突判定
│   │
│   ├── events/              # イベント処理関連
│   │   └── key_press.c      # キー入力処理
│   │
│   ├── utils/               # ユーティリティ関数
│   │   ├── error_handling.c # エラー処理
│   │   └── memory_utils.c   # メモリ管理
│   │
│   └── cleanup/             # クリーンアップ関連
│       ├── exit_game.c      # ゲーム終了処理
│       └── free_all.c       # メモリ解放
│
├── bonus/                  # ボーナスパート（オプション）
│   ├── minimap.c           # ミニマップ実装
│   ├── doors.c             # ドア機能
│   ├── sprites.c           # スプライト機能
│   └── mouse_rotation.c    # マウス回転機能
│
├── assets/                 # テクスチャなどの素材
│   ├── textures/
│   │   ├── north.xpm       # 北壁テクスチャ
│   │   ├── south.xpm       # 南壁テクスチャ
│   │   ├── east.xpm        # 東壁テクスチャ
│   │   └── west.xpm        # 西壁テクスチャ
│   └── maps/
│       ├── test_map.cub    # テスト用マップ
│       └── game_map.cub    # ゲーム用マップ
│
└── libft/                  # libftライブラリ
```