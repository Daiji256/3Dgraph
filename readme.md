# 2変数関数の描画プログラム（3Dgraph）

## 環境

このプログラムはgcc + freeglutの環境でコンパイル可能する．
```
gcc main.c mkfunc.c -o 3Dgraph -lGL -lGLU -lglut -lm -Wall
gcc main.c eval.c -o 3Dgraph -lGL -lGLU -lglut -lm -Wall
```

## 使い方

次のようにコマンドライン引数を設定して実行する．
```
./3Dgraph equation x_min x_max x_interval y_min y_max y_interval
```
例）
```
./3Dgraph "x*x - y*y" -2 2 0.2 -2 2 0.2
./3Dgraph "x*x - y*y" -2 2 -2 2
```

|キー|動作                            |
|----|--------------------------------|
|x   |x軸方向に大きくする             |
|X   |x軸方向に小さくする             |
|y   |y軸方向に大きくする             |
|Y   |y軸方向に小さくする             |
|z   |z軸方向に大きくする             |
|Z   |Z軸方向に小さくする             |
|w   |ワイヤフレームの表示・非表示    |
|s   |サーフェイスモデルの表示・非表示|
|t   |目盛りの表示・非表示            |
|Esc |プログラムの終了                |

マウスの左ドラッグでグラフの回転できる．
マウスのホイールでグラフの拡大，縮小できる．

## 違う環境で使うためには

主に`mkfunc.c`の`compile`を変更することで違う環境でも使える．
C言語にはevalが無いため，コマンドライン引数の数式からソースコードを生成・コンパイルし疑似的に動的な数式の設定をしている．
