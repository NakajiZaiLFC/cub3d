UNAME_S := $(shell uname -s)

LINUX_URL = https://cdn.intra.42.fr/document/document/35303/minilibx-linux.tgz
DARWIN_URL = https://cdn.intra.42.fr/document/document/35302/minilibx_macos_metal.tgz

LINUX_FILE = minilibx-linux.tgz
DARWIN_FILE = minilibx_macos_metal.tgz

setup-minilibx:
ifeq ($(UNAME_S),Linux)
	@echo "Linux環境を検出しました。MinilibX for Linuxをダウンロードします..."
	@curl -o $(LINUX_FILE) $(LINUX_URL)
	@tar -xzf $(LINUX_FILE)
	@if [ -d minilibx-linux ]; then \
		mv minilibx-linux minilibx; \
	elif [ -d mlx ]; then \
		mv mlx minilibx; \
	else \
		echo "エラー: 解凍されたディレクトリが見つかりません"; \
		exit 1; \
	fi
	rm -f $(LINUX_FILE)
	cd minilibx && make
else ifeq ($(UNAME_S),Darwin)
	@echo "Mac環境を検出しました。MinilibX for macOSをダウンロードします..."
	@curl -o $(DARWIN_FILE) $(DARWIN_URL)
	@tar -xzf $(DARWIN_FILE)
	@if [ -d minilibx_macos ]; then \
		mv minilibx_macos minilibx; \
	elif [ -d mlx ]; then \
		mv mlx minilibx; \
	else \
		echo "エラー: 解凍されたディレクトリが見つかりません"; \
		exit 1; \
	fi
	rm -f $(DARWIN_FILE)
	cd minilibx && make
else
	@echo "エラー: サポートされていないOS $(UNAME_S)"
	@exit 1
endif


.PHONY: setup-minilibx