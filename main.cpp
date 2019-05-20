#include "DXlib.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_SIZE_X 640 // �f�t�H���g�̉�ʂ̕�
#define SCREEN_SIZE_Y 480 // �f�t�H���g�̉�ʂ̍���

// �O���[�o���ϐ�
int endFlag;
int stone[8][8];
int endflag = 0;
int turn = -1; // �U�߂̎�ԁB������J�n
int coolTimer = 0;

			   // �֐�
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


// �Ֆʂ�������ԂɃ��Z�b�g����֐�
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


// �t�H���g�̓ǂݍ��݂�����֐�
void readFont() {
	LPCSTR font_path = "JKG-L_3.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}
	ChangeFont("JK�S�V�b�NL"); // �t�H���g�̕ύX
}


// �I�Z���̃{�[�h��`�悷��֐�
void board() {
	int i, a = 0;
	DrawBox(0, 0, 640, 480, GetColor(0, 0, 0), TRUE);
	DrawBox(80, 0, 560, 480, GetColor(40, 175, 12), TRUE); // �w�i�̗�
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

	// �p�X�{�^��
	DrawBox(568, 182, 632, 298, GetColor(125, 125, 125), TRUE);
	DrawString(581, 233, "PASS", GetColor(0, 0, 0));

	// �G���h�{�^��
	DrawBox(8, 182, 72, 298, GetColor(125, 125, 125), TRUE);
	DrawString(28, 233, "End", GetColor(0, 0, 0));
}


// �G���h�{�^���̔���p�̊֐�
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


// ���΂ƍ��΂�Ֆʂɕ`�悷��֐�
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


// �Q�[�����I�����Ă��邩���f����֐�
// �Q�[�����I�����Ă����1�A�܂��r���ł����0��Ԃ��B
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

	if (field0 == 0) return 1; // �Ֆʂ�0�����݂��Ȃ�������1��Ԃ�
	else { // 0�����݂����ꍇ�ȉ��̔�����s��
		if (whiteStone == 0) return 1; // ���̐΂����݂��Ȃ��ꍇ�͏I����1��Ԃ�
		if (brackStone == 0) return 1; // ���̐΂����݂��Ȃ��ꍇ�͏I����1��Ԃ�
		return 0;
	}
	return 1;
}


// �����ɃR�}��u�����Ƃ��ł��邩���ʂ���֐�(1�Œu���āA0�ł����ɂ͐΂�u�����Ƃ��ł��Ȃ�)
int checkAttack(int myX, int myY) {
	int teban = turn;
	int i, j, k, teisuX = 0, teisuY = 0;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			k = 8;
			// �������΂�u�����R�}�̏������΂�
			if (i == 1 && j == 1) continue;

			if (teban == 1) { // �u�������΂����������ꍇ
				switch (i * 10 + j) {
				case 0: // ����̏ꍇ
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

				case 1: // ��
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

				case 2: // ����
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

				case 10: // ��
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

				case 12: // ��
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

				case 20: // �E��
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

				case 21: // �E
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

				case 22: // �E��
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
			else if (teban == -1) { // �u�������΂����������ꍇ
				switch (i * 10 + j) {
				case 0: // ����̏ꍇ
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

				case 1: // ��
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

				case 2: // ����
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

				case 10: // ��
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

				case 12: // ��
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

				case 20: // �E��
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

				case 21: // �E
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

				case 22: // �E��
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
				DrawString(0, 0, "�G���[���������Ă��܂�", GetColor(100, 100, 100));
				return 0;
			}
		}
	}
	return 0;
}


// �����F�ŋ��܂ꂽ�΂𔽓]������
void rebarse(int myX, int myY) {
	int teban = turn;
	int i, j, k, teisuX = 0, teisuY = 0;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			// �������΂�u�����R�}�̏������΂�
			if (i == 1 && j == 1) continue;

			if (teban == 1) { // �u�������΂����������ꍇ
				switch (i * 10 + j) {
				case 0: // ����̏ꍇ
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

				case 1: // ��
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

				case 2: // ����
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

				case 10: // ��
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

				case 12: // ��
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

				case 20: // �E��
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

				case 21: // �E
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

				case 22: // �E��
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
			else if (teban == -1) { // �u�������΂����������ꍇ
				switch (i * 10 + j) {
				case 0: // ����̏ꍇ
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

				case 1: // ��
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

				case 2: // ����
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

				case 10: // ��
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

				case 12: // ��
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

				case 20: // �E��
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

				case 21: // �E
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

				case 22: // �E��
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


// �N���b�N���ꂽ�}�X�ɐ΂�u���֐�
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
						DrawString(20, 20, "�����ɐ΂͒u���܂���I", GetColor(255, 0, 0));
					}
				}
			}
		}
	}
}


// �p�X�{�^���̔���p�̊֐�
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


// ��Ԃ�甒���̐΂̌���\������֐�
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

	sprintf_s(blackStone, 128, "���� :%d", black);
	sprintf_s(whiteStone, 128, "���� :%d", white);

	DrawString(570, 320, blackStone, GetColor(255, 255, 255));
	DrawString(570, 340, whiteStone, GetColor(255, 255, 255));
}


// window�̃T�C�Y���t���X�N���[���ɂ��邩�I�������o���֐�
void windowSize() {
	int i, coolTimer = 0, nagaosiTimer = 0, scleen = 1;
	double now = 0, goal = 0;

	while (1) {
		LoadGraphScreen(0, 0, "picture\\�w�i1.png", TRUE);
		if (CheckHitKey(KEY_INPUT_SPACE)) break;
		if (scleen == 1) {
			goal = 0;
			DrawString(105, 80, "�t���X�N���[�����[�h�ɂ���", GetColor(255, 255, 255));
			DrawString(100, 120, "�E�B���h�E���[�h�ɂ���", GetColor(255, 255, 255));
			if (now != 0) {
				now += (goal - now) / 5;
				if (now < 0.5) now = 0;
			}
		}
		else if (scleen == -1) {
			goal = 40;
			DrawString(100, 80, "�t���X�N���[�����[�h�ɂ���", GetColor(255, 255, 255));
			DrawString(105, 120, "�E�B���h�E���[�h�ɂ���", GetColor(255, 255, 255));
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

	if (scleen == 1) ChangeWindowMode(FALSE); // �t���X�N���[�����[�h
	if (scleen == -1) ChangeWindowMode(TRUE); // �E�B���h�E���[�h
	LoadGraphScreen(0, 0, "picture\\�w�i1.png", TRUE);
	ScreenFlip();
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++             +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  ( WinMain )  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++             +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;//�������Ɨ���ʉ�
	windowSize();
	readFont(); // �t�H���g�ǂݍ���
	SetMouseDispFlag(TRUE);

	while (1) {
		boardReset(); // �Ֆʂ�������ԂɃ��Z�b�g
		while (checkEnd() == 0) {
			board(); // �Ֆʂ̊i�q��`��
			setStone(); // �N���b�N�����ꏊ�ɐ΂�u��
			passBottan(); // �p�X�Ɋւ��鏈��
			gameEndBottan(); // �Q�[�����I��������֐�
			state(); // ���݂̎�ԂȂǂ̃X�e�[�^�X��\��
			drawStone(); // �Ֆʂɐ΂�\��
			ScreenFlip(); // ���̃X�N���[����\�ɓ���
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) return 0; // �G�X�P�[�v�L�[�ŋ����I��
		}
		DrawBox(20, 225, 620, 255, GetColor(255, 255, 255), TRUE);
		DrawString(40, 233, "������x�ΐ킷��ꍇ�ɂ�'y'���A��߂�ꍇ�ɂ�'n'����͂��Ă��������B", GetColor(0, 0, 0));
		ScreenFlip(); // ���̃X�N���[����\�ɓ���
		WaitKey();
		if (CheckHitKey(KEY_INPUT_Y)) boardReset(); // �Ֆʂ�������ԂɃ��Z�b�g
		else if (CheckHitKey(KEY_INPUT_N) || CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}
	DxLib_End(); // DX���C�u�����̏I��
	return 0;
}
