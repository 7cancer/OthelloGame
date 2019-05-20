#include "DXlib.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_SIZE_X 640 // デフォルトの画面の幅
#define SCREEN_SIZE_Y 480 // デフォルトの画面の高さ

// グローバル変数
int endFlag;
int stone[8][8];
int endflag = 0;
int turn = -1; // 攻めの手番。黒から開始
int coolTimer = 0;

			   // 関数
void boardReset();
void board();
void drawStone();
void setStone();
int checkEnd();
void passBottan();
void gameEndBottan();
void state();
int checkAttack();
void rebarse();


// 盤面を初期状態にリセットする関数
void boardReset() {
	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			stone[i][j] = 0;
		}
	}
	endflag = 0;
	stone[3][3] = 1;
	stone[4][4] = 1;
	stone[3][4] = -1;
	stone[4][3] = -1;
}


// フォントの読み込みをする関数
void readFont() {
	LPCSTR font_path = "JKG-L_3.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	ChangeFont("JKゴシックL"); // フォントの変更
}


// オセロのボードを描画する関数
void board() {
	int i, a = 0;
	DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
	DrawBox(80, 0, 560, 480, GetColor(40, 175, 12), TRUE); // 背景の緑
	DrawBox(80, 0, 560, 480, GetColor(255, 255, 255), FALSE);
	DrawBox(88, 8, 552, 472, GetColor(255, 255, 255), FALSE);
	DrawCircle(204, 124, 3, GetColor(255, 255, 255), TRUE);
	DrawCircle(436, 124, 3, GetColor(255, 255, 255), TRUE);
	DrawCircle(436, 356, 3, GetColor(255, 255, 255), TRUE);
	DrawCircle(204, 356, 3, GetColor(255, 255, 255), TRUE);
	for (i = 0; i < 7; i++) {
		a += 58;
		DrawLine(88, 8 + a, 552, 8 + a, GetColor(255, 255, 255));
		DrawLine(88 + a, 8, 88 + a, 472, GetColor(255, 255, 255));
	}

	// パスボタン
	DrawBox(568, 182, 632, 298, GetColor(125, 125, 125), TRUE);
	DrawString(581, 233, "PASS", GetColor(0, 0, 0));

	// エンドボタン
	DrawBox(8, 182, 72, 298, GetColor(125, 125, 125), TRUE);
	DrawString(28, 233, "End", GetColor(0, 0, 0));
}


// エンドボタンの判定用の関数
void gameEndBottan() {
	int mouseX, mouseY;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		GetMousePoint(&mouseX, &mouseY);
		if (8 <= mouseX && mouseX <= 72) {
			if (182 <= mouseY && mouseY <= 298) {
				endflag = 1;
			}
		}
	}
}


// 白石と黒石を盤面に描画する関数
void drawStone() {
	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (stone[i][j] == 1) {
				DrawCircle(117 + (i * 58), 37 + (j * 58), 26, GetColor(255, 255, 255), TRUE);
			}
			else if (stone[i][j] == -1) {
				DrawCircle(117 + (i * 58), 37 + (j * 58), 26, GetColor(0, 0, 0), TRUE);
			}
		}
	}
}


// ゲームが終了しているか判断する関数
// ゲームが終了していれば1、まだ途中であれば0を返す。
int checkEnd() {
	int i, j, field0 = 0, whiteStone = 0, brackStone = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (stone[i][j] == 0) field0++;
			if (stone[i][j] == 1) whiteStone++;
			if (stone[i][j] == -1) brackStone++;
		}
	}
	if (endflag == 1) return 1;

	if (field0 == 0) return 1; // 盤面に0が存在しなかったら1を返す
	else { // 0が存在した場合以下の判定を行う
		if (whiteStone == 0) return 1; // 白の石が存在しない場合は終了の1を返す
		if (brackStone == 0) return 1; // 黒の石が存在しない場合は終了の1を返す
		return 0;
	}
	return 1;
}


// そこにコマを置くことができるか判別する関数(1で置けて、0でそこには石を置くことができない)
int checkAttack(int myX, int myY) {
	int teban = turn;
	int i, j, k, teisuX = 0, teisuY = 0;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			k = 8;
			// 自分が石を置いたコマの処理を飛ばす
			if (i == 1 && j == 1) continue;

			if (teban == 1) { // 置きたい石が白だった場合
				switch (i * 10 + j) {
				case 0: // 左上の場合
					teisuX = -1, teisuY = -1;
					if (myX < myY) k = myX - 1;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY - 1] == 1) return 1;
						}
						else break;
						teisuX--, teisuY--;
					}
					break;

				case 1: // 左
					teisuX = -1, teisuY = 0;
					k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY] == 1) return 1;
						}
						else break;
						teisuX--;
					}
					break;

				case 2: // 左下
					teisuX = -1, teisuY = 1;
					if (myX > 7 - myY) k = 6 - myY;
					else k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY + 1] == 1) return 1;
						}
						else break;
						teisuX--, teisuY++;
					}
					break;

				case 10: // 上
					teisuX = 0, teisuY = -1;
					k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX][myY + teisuY - 1] == 1) return 1;
						}
						else break;
						teisuY--;
					}
					break;

				case 12: // 下
					teisuX = 0, teisuY = 1;
					k = 6 - myY;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX][myY + teisuY + 1] == 1) return 1;
						}
						else break;
						teisuY++;
					}
					break;

				case 20: // 右上
					teisuX = 1, teisuY = -1;
					if (7 - myX < myY) k = 6 - myX;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY - 1] == 1) return 1;
						}
						else break;
						teisuX++, teisuY--;
					}
					break;

				case 21: // 右
					teisuX = 1, teisuY = 0;
					k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY] == 1) return 1;
						}
						else break;
						teisuX++;
					}
					break;

				case 22: // 右下
					teisuX = 1, teisuY = 1;
					if (7 - myX > 7 - myY) k = 6 - myY;
					else k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY + 1] == 1) return 1;
						}
						else break;
						teisuX++, teisuY++;
					}
					break;
				}
			}
			else if (teban == -1) { // 置きたい石が黒だった場合
				switch (i * 10 + j) {
				case 0: // 左上の場合
					teisuX = -1, teisuY = -1;
					if (myX < myY) k = myX - 1;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY - 1] == -1) return 1;
						}
						else break;
						teisuX--, teisuY--;
					}
					break;

				case 1: // 左
					teisuX = -1, teisuY = 0;
					k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY] == -1) return 1;
						}
						else break;
						teisuX--;
					}
					break;

				case 2: // 左下
					teisuX = -1, teisuY = 1;
					if (myX > 7 - myY) k = 6 - myY;
					else k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY + 1] == -1) return 1;
						}
						else break;
						teisuX--, teisuY++;
					}
					break;

				case 10: // 上
					teisuX = 0, teisuY = -1;
					k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX][myY + teisuY - 1] == -1) return 1;
						}
						else break;
						teisuY--;
					}
					break;

				case 12: // 下
					teisuX = 0, teisuY = 1;
					k = 6 - myY;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX][myY + teisuY + 1] == -1) return 1;
						}
						else break;
						teisuY++;
					}
					break;

				case 20: // 右上
					teisuX = 1, teisuY = -1;
					if (7 - myX < myY) k = 6 - myX;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY - 1] == -1) return 1;
						}
						else break;
						teisuX++, teisuY--;
					}
					break;

				case 21: // 右
					teisuX = 1, teisuY = 0;
					k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY] == -1) return 1;
						}
						else break;
						teisuX++;
					}
					break;

				case 22: // 右下
					teisuX = 1, teisuY = 1;
					if (7 - myX > 7 - myY) k = 6 - myY;
					else k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY + 1] == -1) return 1;
						}
						else break;
						teisuX++, teisuY++;
					}
					break;
				}
			}
			else {
				DrawString(0, 0, "エラーが発生しています", GetColor(100, 100, 100));
				return 0;
			}
		}
	}
	return 0;
}


// 同じ色で挟まれた石を反転させる
void rebarse(int myX, int myY) {
	int teban = turn;
	int i, j, k, teisuX = 0, teisuY = 0;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			// 自分が石を置いたコマの処理を飛ばす
			if (i == 1 && j == 1) continue;

			if (teban == 1) { // 置きたい石が白だった場合
				switch (i * 10 + j) {
				case 0: // 左上の場合
					teisuX = -1, teisuY = -1;
					if (myX < myY) k = myX - 2;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY - 1] == 1) {
								teisuX = -1, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX--, teisuY--;
								}
							}
						}
						else break;
						teisuX--, teisuY--;
					}
					break;

				case 1: // 左
					teisuX = -1, teisuY = 0;
					k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY] == 1) {
								teisuX = -1, teisuY = 0;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX--;
								}
							}
						}
						else break;
						teisuX--;
					}
					break;

				case 2: // 左下
					teisuX = -1, teisuY = 1;
					if (myX > 7 - myY) k = 6 - myY;
					else k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX - 1][myY + teisuY + 1] == 1) {
								teisuX = -1, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX--, teisuY++;
								}
							}
						}
						else break;
						teisuX--, teisuY++;
					}
					break;

				case 10: // 上
					teisuX = 0, teisuY = -1;
					k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX][myY + teisuY - 1] == 1) {
								teisuX = 0, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuY--;
								}
							}
						}
						else break;
						teisuY--;
					}
					break;

				case 12: // 下
					teisuX = 0, teisuY = 1;
					k = 6 - myY;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX][myY + teisuY + 1] == 1) {
								teisuX = 0, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuY++;
								}
							}
						}
						else break;
						teisuY++;
					}
					break;

				case 20: // 右上
					teisuX = 1, teisuY = -1;
					if (7 - myX < myY) k = 6 - myX;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY - 1] == 1) {
								teisuX = 1, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX++, teisuY--;
								}
							}
						}
						else break;
						teisuX++, teisuY--;
					}
					break;

				case 21: // 右
					teisuX = 1, teisuY = 0;
					k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY] == 1) {
								teisuX = 1, teisuY = 0;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX++;
								}
							}
						}
						else break;
						teisuX++;
					}
					break;

				case 22: // 右下
					teisuX = 1, teisuY = 1;
					if (7 - myX > 7 - myY) k = 6 - myY;
					else k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == -1) {
							if (stone[myX + teisuX + 1][myY + teisuY + 1] == 1) {
								teisuX = 1, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == -1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = 1;
									teisuX++, teisuY++;
								}
							}
						}
						else break;
						teisuX++, teisuY++;
					}
					break;
				}
			}
			else if (teban == -1) { // 置きたい石が黒だった場合
				switch (i * 10 + j) {
				case 0: // 左上の場合
					teisuX = -1, teisuY = -1;
					if (myX < myY) k = myX - 1;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY - 1] == -1) {
								teisuX = -1, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX--, teisuY--;
								}
							}
						}
						else break;
						teisuX--, teisuY--;
					}
					break;

				case 1: // 左
					teisuX = -1, teisuY = 0;
					k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY] == -1) {
								teisuX = -1, teisuY = 0;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX--;
								}
							}
						}
						else break;
						teisuX--;
					}
					break;

				case 2: // 左下
					teisuX = -1, teisuY = 1;
					if (myX > 7 - myY) k = 6 - myY;
					else k = myX - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX - 1][myY + teisuY + 1] == -1) {
								teisuX = -1, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX--, teisuY++;
								}
							}
						}
						else break;
						teisuX--, teisuY++;
					}
					break;

				case 10: // 上
					teisuX = 0, teisuY = -1;
					k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX][myY + teisuY - 1] == -1) {
								teisuX = 0, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuY--;
								}
							}
						}
						else break;
						teisuY--;
					}
					break;

				case 12: // 下
					teisuX = 0, teisuY = 1;
					k = 6 - myY;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX][myY + teisuY + 1] == -1) {
								teisuX = 0, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuY++;
								}
							}
						}
						else break;
						teisuY++;
					}
					break;

				case 20: // 右上
					teisuX = 1, teisuY = -1;
					if (7 - myX < myY) k = 6 - myX;
					else k = myY - 1;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY - 1] == -1) {
								teisuX = 1, teisuY = -1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX++, teisuY--;
								}
							}
						}
						else break;
						teisuX++, teisuY--;
					}
					break;

				case 21: // 右
					teisuX = 1, teisuY = 0;
					k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY] == -1) {
								teisuX = 1, teisuY = 0;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX++;
								}
							}
						}
						else break;
						teisuX++;
					}
					break;

				case 22: // 右下
					teisuX = 1, teisuY = 1;
					if (7 - myX > 7 - myY) k = 6 - myY;
					else k = 6 - myX;
					while (k > 0) {
						k--;
						if (stone[myX + teisuX][myY + teisuY] == 1) {
							if (stone[myX + teisuX + 1][myY + teisuY + 1] == -1) {
								teisuX = 1, teisuY = 1;
								while (stone[myX + teisuX][myY + teisuY] == 1) {
									if ((myX + teisuX) <= -1 || (myX + teisuX) >= 8 || (myY + teisuY) <= -1 || (myY + teisuY) >= 8) break;
									stone[myX + teisuX][myY + teisuY] = -1;
									teisuX++, teisuY++;
								}
							}
						}
						else break;
						teisuX++, teisuY++;
					}
					break;
				}
			}
		}
	}
}


// クリックされたマスに石を置く関数
void setStone() {
	int mouseX, mouseY, myX, myY;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		GetMousePoint(&mouseX, &mouseY);
		if (88 <= mouseX && mouseX <= 552) {
			if (8 <= mouseY && mouseY <= 472) {
				myX = (mouseX - 88) / 58;
				myY = (mouseY - 8) / 58;
				if (stone[myX][myY] == 0) {
					if (checkAttack(myX, myY) == 1) {
						stone[myX][myY] = turn;
						rebarse(myX, myY);
						turn *= -1;
					}
					else {
						DrawString(20, 20, "そこに石は置けません！", GetColor(255, 0, 0));
					}
				}
			}
		}
	}
}


// パスボタンの判定用の関数
void passBottan() {
	int mouseX, mouseY;
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		GetMousePoint(&mouseX, &mouseY);
		if (568 <= mouseX && mouseX <= 632) {
			if (182 <= mouseY && mouseY <= 298) {
				if (coolTimer == 0) {
					turn *= -1;
				}
			}
		}
		coolTimer = 1;
	}
	else {
		coolTimer = 0;
	}
}


// 手番やら白黒の石の個数を表示する関数
void state() {
	int black = 0, white = 0;
	int i, j;
	char blackStone[128], whiteStone[128];
	char B[5], W[5];

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (stone[i][j] == -1) black++;
			if (stone[i][j] == 1) white++;
		}
	}

	if (turn == 1) DrawString(581, 33, "WHITE", GetColor(255, 255, 255));
	else DrawString(581, 33, "BLACK", GetColor(255, 255, 255));

	sprintf_s(blackStone, 128, "黒石 :%d", black);
	sprintf_s(whiteStone, 128, "白石 :%d", white);

	DrawString(570, 320, blackStone, GetColor(255, 255, 255));
	DrawString(570, 340, whiteStone, GetColor(255, 255, 255));
}


// windowのサイズをフルスクリーンにするか選択肢を出す関数
void windowSize() {
	int i, coolTimer = 0, nagaosiTimer = 0, scleen = 1;
	double now = 0, goal = 0;

	while (1) {
		LoadGraphScreen(0, 0, "picture\\背景1.png", TRUE);
		if (CheckHitKey(KEY_INPUT_SPACE)) break;
		if (scleen == 1) {
			goal = 0;
			DrawString(105, 80, "フルスクリーンモードにする", GetColor(255, 255, 255));
			DrawString(100, 120, "ウィンドウモードにする", GetColor(255, 255, 255));
			if (now != 0) {
				now += (goal - now) / 5;
				if (now < 0.5) now = 0;
			}
		}
		else if (scleen == -1) {
			goal = 40;
			DrawString(100, 80, "フルスクリーンモードにする", GetColor(255, 255, 255));
			DrawString(105, 120, "ウィンドウモードにする", GetColor(255, 255, 255));
			if (now != 40) {
				now += (goal - now) / 5;
				if (now > 39.5) now = 40;
			}
		}
		DrawString(70, 80 + now, "->", GetColor(255, 255, 255));

		if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN)) {
			nagaosiTimer++;
			if (coolTimer == 1 || nagaosiTimer > 45) {
				scleen = -1 * scleen;
				coolTimer = 0;
			}
		}
		else {
			coolTimer = 1;
			nagaosiTimer = 0;
		}

		ScreenFlip();
	}

	if (scleen == 1) ChangeWindowMode(FALSE); // フルスクリーンモード
	if (scleen == -1) ChangeWindowMode(TRUE); // ウィンドウモード
	LoadGraphScreen(0, 0, "picture\\背景1.png", TRUE);
	ScreenFlip();
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++             +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  ( WinMain )  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++             +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;//初期化と裏画面化
	windowSize();
	readFont(); // フォント読み込み
	SetMouseDispFlag(TRUE);

	while (1) {
		boardReset(); // 盤面を初期状態にリセット
		while (checkEnd() == 0) {
			board(); // 盤面の格子を描画
			setStone(); // クリックした場所に石を置く
			passBottan(); // パスに関する処理
			gameEndBottan(); // ゲームを終了させる関数
			state(); // 現在の手番などのステータスを表示
			drawStone(); // 盤面に石を表示
			ScreenFlip(); // 裏のスクリーンを表に投射
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) return 0; // エスケープキーで強制終了
		}
		DrawBox(20, 225, 620, 255, GetColor(255, 255, 255), TRUE);
		DrawString(40, 233, "もう一度対戦する場合には'y'を、やめる場合には'n'を入力してください。", GetColor(0, 0, 0));
		ScreenFlip(); // 裏のスクリーンを表に投射
		WaitKey();
		if (CheckHitKey(KEY_INPUT_Y)) boardReset(); // 盤面を初期状態にリセット
		else if (CheckHitKey(KEY_INPUT_N) || CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}
	DxLib_End(); // DXライブラリの終了
	return 0;
}
