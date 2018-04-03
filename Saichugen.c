/******************************************************************************
 *
 *        目的：最中限で予め書かれた戦略の勝敗や勝率を表示する
 *
 *        制作：明星大学情報学部情報学科CSコース　丸山研究室 2年生(2017年)
 *
 *   　　　 　要件・設計： 柴田
 *　　　　　　戦略設計：   秋野・印部・柴田
 *        　　テスト：	   印部・柴田
 *
 *        制作期間：14日間
 *
 *        使用方法：プログラムを実行
 *		    オプション -DDEBUG　でデバック表示
 * 
 *	  注意：戦略を書く際に、必ずsaveArrayとsortedArrayに戻り値の代入
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int sumWin[3]; 			//各々の勝ち回数(全てのラウンドに引き継ぐ)
int saveArray[3]; 		//ソート以前の状態を保存
int sortedArray[3]; 		//ソート後の状態を保存
int countWinRound[3]; 		//ラウンド毎の勝者に+1

/* ラウンド毎にゼロクリアする配列 */
int tempHandsArray[13][4]; 	//全てのカードを配り終えたかを確認(0...なし,1...あり)
int putDownCards[15][3]; 	//ターン毎に出したカードを保存

typedef struct infoPlayer{
	int score; 		//勝ち点の合計
	char position[4]; 	//相対的な立ち位置(min, mid, max)
	int rank; 		//前のターンの順位
	int hands[13][4]; 	//カードの有無(0...なし, 1...あり)
	int handsList[17]; 	//カードの数字とマークに重み付けした値
} info;


/*----------------------------          勝敗判定          ---------------------------*/ 

//ターンの勝利者判定
void chooseWinner(char *tempStr, info *array)
{
	int i;

	i = 0;
	while(sortedArray[1] != saveArray[i]){
		if(i == 2){
			break;
		}
		++i;
	}

	switch(i){
		case 0:
			array[0].score += sortedArray[1] / 10;
			sumWin[0] += 1;
			strcpy(tempStr, "player1"); 
			break;
		case 1:
			array[1].score += sortedArray[1] / 10;
			sumWin[1] += 1;
			strcpy(tempStr, "player2"); 
			break;
		case 2:
			array[2].score += sortedArray[1] / 10;
			sumWin[2] += 1;
			strcpy(tempStr, "player3"); 	
			break;
	}
}

//ラウンド勝利者判定(1ラウンド毎)
void roundWinner(char *tempStr, info *array)
{
	int i;

	if(saveArray[0] == saveArray[1] && saveArray[0] == saveArray[2] && saveArray[1] == saveArray[2]){
		strcpy(tempStr, "player1, player2, player3");
		countWinRound[0] += 1;
		countWinRound[1] += 1;
		countWinRound[2] += 1;
	}else if(saveArray[0] == saveArray[1]){
		strcpy(tempStr, "player1, player2");
		countWinRound[1] += 1;
		countWinRound[2] += 1;
	}else if(saveArray[0] == saveArray[2]){
		strcpy(tempStr, "player1, player3");
		countWinRound[0] += 1;
		countWinRound[2] += 1;
	}else if(saveArray[1] == saveArray[2]){
		strcpy(tempStr, "player2, player3");
		countWinRound[1] += 1;
		countWinRound[2] += 1;
	}else{
		i = 0;
		while(sortedArray[1] != saveArray[i]){
			if(i == 2){
				break;
			}
			++i;
		}

		switch(i){
			case 0:	strcpy(tempStr, "player1"); countWinRound[0] += 1; break;
			case 1:	strcpy(tempStr, "player2"); countWinRound[1] += 1; break;
			case 2:	strcpy(tempStr, "player3"); countWinRound[2] += 1; break;
		}
	}
}


//昇順ソート
void sortNum()
{
	int tempNum;

	if(sortedArray[0] > sortedArray[1]){
		tempNum = sortedArray[0];
		sortedArray[0] = sortedArray[1];
		sortedArray[1] = tempNum;
	}
	if(sortedArray[0] > sortedArray[2]){
		tempNum = sortedArray[0];
		sortedArray[0] = sortedArray[2];
		sortedArray[2] = tempNum;
	}
	if(sortedArray[1] > sortedArray[2]){
		tempNum = sortedArray[1];
		sortedArray[1] = sortedArray[2];
		sortedArray[2] = tempNum;
	}
}


//ランキングを調べる
void nowRank(info *array)
{

	int i, j, min, mid, max;
	int sub1, sub2;

	for(i = 0; i < 3; ++i){
		sortedArray[i] = array[i].score;
		saveArray[i] = array[i].score;
	}

	sortNum();

#ifdef DEBUG
	for(i = 0; i < 3; ++i){
		printf("sortedArray[%d] = %d\n", i, sortedArray[i]);
	}
#endif

	if(sortedArray[1] == sortedArray[0] || sortedArray[1] == sortedArray[2]){
		if(sortedArray[1] == sortedArray[0] && sortedArray[1] == sortedArray[2]){
			array[0].rank = 1;
			array[1].rank = 1;
			array[2].rank = 1;
			strcpy(array[0].position, "mid");
			strcpy(array[1].position, "mid");
			strcpy(array[2].position, "mid");
		}else if(sortedArray[1] == sortedArray[0]){
			j = 0;
			while(sortedArray[2] != saveArray[j]){
				++j;
			}
			if(j == 3){
				--j;
			}

			array[j].rank = 3;
			array[(j + 1) % 3].rank = 1;
			array[(j + 2) % 3].rank = 1;
			strcpy(array[j].position, "max");
			strcpy(array[(j + 1) % 3].position, "min");
			strcpy(array[(j + 2) % 3].position, "min");

		}else if(sortedArray[1] == sortedArray[2]){
			j = 0;
			while(sortedArray[0] != saveArray[j]){
				++j;
			}
			if(j == 3){
				--j;
			}

			array[j].rank = 3;
			array[(j + 1) % 3].rank = 1;
			array[(j + 2) % 3].rank = 1;
			strcpy(array[j].position, "min");
			strcpy(array[(j + 1) % 3].position, "max");
			strcpy(array[(j + 2) % 3].position, "max");		}
	}else{
		sub1 = sortedArray[1] - sortedArray[0];
		sub2 = sortedArray[2] - sortedArray[1];

		mid  = 0;
		while(sortedArray[1] != saveArray[mid]){
			++mid;
		}
		if(mid == 3){
			mid = 2;
		}

		min = 0;
		while(sortedArray[0] != saveArray[min]){
			++min;
		}
		if(min == 3){
			min = 2;
		}

		max = 0;
		while(sortedArray[2] != saveArray[max]){
			++max;
		}
		if(max == 3){
			max = 2;	
		}

#ifdef DEBUG
		printf("sub1 = %d\n", sub1);
		printf("sub2 = %d\n", sub2);
		printf("min = %d\n", min);
		printf("max = %d\n", max);
		printf("mid = %d\n", mid);
#endif

		if(sub1 == sub2){
			array[mid].rank = 1;
			array[min].rank = 2;
			array[max].rank = 2;
			strcpy(array[mid].position, "mid");
			strcpy(array[min].position, "min");
			strcpy(array[max].position, "max");
		}else{
			if(sub1 < sub2){
				array[mid].rank = 1;
				array[min].rank = 2;
				array[max].rank = 3;
				strcpy(array[mid].position, "mid");
				strcpy(array[min].position, "min");
				strcpy(array[max].position, "max");
			}
			if(sub1 > sub2){
				array[mid].rank = 1;
				array[max].rank = 2;
				array[min].rank = 3;
				strcpy(array[mid].position, "mid");
				strcpy(array[max].position, "max");
				strcpy(array[min].position, "min");
			}
		}
	}
}


//////////////////       手札配布         ////////////////////


//数字とマークに重み付け
//[4] > [3] > [2] > [1]
//スペード8 > ハート6 > ダイヤ4 > クラブ2
//ナンバー10スペード = (9 + 1) * 10 + (4 + 1) * 2
void handsList(info *array){
	int p1, p2, p3;
	int i, j;

	p1 = 0;
	p2 = 0;
	p3 = 0;

	for(i = 0; i < 13; ++i){
		for(j = 0; j < 4; ++j){
			if(array[0].hands[i][j]){
				array[0].handsList[p1++] = (i + 1) * 10 + (j + 1) * 2; 
				continue;
			}
			if(array[1].hands[i][j]){
				array[1].handsList[p2++] = (i + 1) * 10 + (j + 1) * 2;
				continue;
			}
			if(array[2].hands[i][j]){
				array[2].handsList[p3++] = (i + 1) * 10 + (j + 1) * 2; 
				continue;
			}
		}
	}
}

//手札のカードの有無判定とランダムな数字を割当
void serverHands2(int count, info *array)
{
	int tempNum, tempMark;

	tempNum = rand() % 13;
	while(tempHandsArray[tempNum][0] && tempHandsArray[tempNum][1] && tempHandsArray[tempNum][2] && tempHandsArray[tempNum][3]){
		tempNum = rand() % 13;
	}
	tempMark = rand() % 4;
	while(tempHandsArray[tempNum][tempMark]){
		tempMark = rand() % 4;
	}
	tempHandsArray[tempNum][tempMark] = 1;

	switch(count){
		case 0: array[0].hands[tempNum][tempMark] = 1; break;
		case 1: array[1].hands[tempNum][tempMark] = 1; break;
		case 2: array[2].hands[tempNum][tempMark] = 1; break;
	}
}

//手札17枚配布
void serverHands1(info *array)
{
	int i;

	i = 0;
	while(i < 17){
		serverHands2(0, array);
		serverHands2(1, array);
		serverHands2(2, array);
		++i;
	}
}

//array[playerNum].hands[][]の最大値を重み付けして返す
int mostMin(int playerNum, info *array)
{
	int i, j = 0;
	int min;
	int ifBreak = 0;

	for(i = 0; i < 13; i++){
		if(array[playerNum].hands[i][0] == 0 && array[playerNum].hands[i][1] == 0 &&  array[playerNum].hands[i][2] == 0){
			continue;
		}
		for(j = 0; j < 4; j++){
			if(array[playerNum].hands[i][j] == 1){
				ifBreak = 1;
				break;
			}
		}
		if(ifBreak){
			break;
		}
	}
	min = (i + 1) * 10 + (j + 1) * 2;

	return min;
}

//array[playerNum].hands[][]の最小値を重み付けして返す
int mostMax(int playerNum, info *array)
{
	int i, j = 0;
	int max;
	int ifBreak = 0;

	for(i = 12; i >= 0; i--){
		if(array[playerNum].hands[i][0] == 0 && array[playerNum].hands[i][1] == 0 && array[playerNum].hands[i][2] == 0){
			continue;
		}
		for(j = 3; j >= 0; j--){
			if(array[playerNum].hands[i][j] == 1){
				ifBreak = 1;
				break;
			}
		}
		if(ifBreak){
			break;
		}
	}
	max = (i + 1) * 10 + (j + 1) * 2;

	return max;
}

//重み付けされた値をarray[playerNum].hands[][]から削除(28 -->> array[playernum].hands[1][3] = 0)
void delValue(int num, int playerNum, info *array)
{
	int i, j;

	i = num / 10 - 1;
	j = num % 10 / 2 - 1;

	array[playerNum].hands[i][j] = 0;
}

/////////////         戦略          /////////////


//ランダムで出す
int stratRandom(int playerNum, info *array)
{
	int tempValue, tempNum, tempMark;

	tempNum = rand() % 13;
	while(array[playerNum].hands[tempNum][0] == 0 &&  array[playerNum].hands[tempNum][1] == 0 && array[playerNum].hands[tempNum][2] == 0 &&  array[playerNum].hands[tempNum][3] == 0){
		tempNum = rand() % 13;
	}
	tempMark = rand() % 4;
	while( !array[playerNum].hands[tempNum][tempMark]){
		tempMark = rand() % 4;
	}
	tempValue = (tempNum + 1) * 10 + (tempMark + 1) * 2;
	array[playerNum].hands[tempNum][tempMark] = 0;

	saveArray[playerNum] = tempValue; 
	sortedArray[playerNum] = tempValue; 

	return tempValue;
}

//昇順に出す
int strat123(int playerNum, info *array)
{
	static int i;

	//2ラウンド以上の開始時に代入
	if(i == 15){
		i = 0;
	}

	saveArray[playerNum] = array[playerNum].handsList[i]; 
	sortedArray[playerNum] = array[playerNum].handsList[i];

	return array[playerNum].handsList[i++];
}

//1,17,2,16,3...最大値と最小値を交互に出す
int upDown(int playerNum, info *array)
{
	static int upNum = 0;
	static int downNum = 16;

	//2ラウンド以上の時に代入
	if(upNum == 8){
		upNum = 0;
		downNum = 16;
	}

	if((downNum - upNum) % 2){
		saveArray[playerNum] = array[playerNum].handsList[upNum];
		sortedArray[playerNum] = array[playerNum].handsList[upNum];
		return array[playerNum].handsList[upNum++];
	}else{
		saveArray[playerNum] = array[playerNum].handsList[downNum];
		sortedArray[playerNum] = array[playerNum].handsList[downNum];
		return array[playerNum].handsList[downNum--];
	}
}

//相手二人の出た目の平均に近い値を出す(なければランダム)
int aveEnemyHands(int playerNum, info *array)
{
	int i;
	int arrayValue = 0, aveEnemy;
	int min;
	static int count;

	//2ラウンド以上でゼロクリア
	if(count == 15){
		count = 0;
	}

	if(count == 0){
		arrayValue = stratRandom(playerNum, array);
	}else{
		aveEnemy = (putDownCards[count - 1][(playerNum + 1) % 3] + putDownCards[count - 1][(playerNum + 2) % 3]) / 2;

		min = aveEnemy / 10;

		i = 0;
		while(array[playerNum].hands[min - 1][i] == 0 && i < 4){
			++i;
		}
		if(i == 4){
			arrayValue = stratRandom(playerNum, array);
		}
		if(i >= 0 && i < 4){
			arrayValue = min * 10 + (i + 1) * 2; 
			array[playerNum].hands[min - 1][i] = 0;
		}
	}

	++count;
	saveArray[playerNum] = arrayValue;
	sortedArray[playerNum] = arrayValue;

	return arrayValue;
}

//相手の平均に近い値を出す(2ターン以降はランダムなし)
int halfAndHalf(int playerNum, info *array)
{
	int i, j;
	int arrayValue = 0, aveEnemy;
	int tempNum, tempMark,breakflag = 0;
	static int count;

	//2ラウンド以上にゼロクリア
	if(count == 15){
		count = 0;
	}

	if(count == 0){
		arrayValue = stratRandom(playerNum, array);
	}else{
		aveEnemy = (putDownCards[count - 1][(playerNum + 1) % 3] + putDownCards[count - 1][(playerNum + 2) % 3]) / 2;

		//aveEnemyが奇数->偶数
		if(aveEnemy % 2){
			aveEnemy += 1;
		}

		//10,20,30等の10の倍数に+2
		if(aveEnemy % 10 == 0){
			aveEnemy += 2;
		}

		tempNum = aveEnemy / 10;
		tempMark = (aveEnemy % 10) / 2;

		if(array[playerNum].hands[tempNum - 1][tempMark - 1] == 1){
			array[playerNum].hands[tempNum -1][tempMark -1] = 0;
			arrayValue = tempNum*10+(tempMark)*2;
		}else{
			for ( i = 0;i < 4; i++){
				for(j  = 0;j < 13;j++){
					if(array[playerNum].hands[j][tempMark -1] == 1)	{
						breakflag = 1;
						arrayValue = (j + 1)*10+(tempMark)*2;
						array[playerNum].hands[j][tempMark -1] = 0;
						break;
					}else if(array[playerNum].hands[tempNum-1][i] == 1){
						breakflag = 1;
						arrayValue = tempNum*10+(i+1)*2;
						array[playerNum].hands[tempNum-1][i] = 0;
						break;
					}else if(array[playerNum].hands[j][i] == 1){
						breakflag = 1;
						arrayValue = (j+1)*10+(i+1)*2;
						array[playerNum].hands[j][i] = 0;
						break;
					}
				}
				if(breakflag == 1){
					break;
				}
			}
		}
	}

	++count;
	saveArray[playerNum] = arrayValue;
	sortedArray[playerNum] = arrayValue;

	return arrayValue;
}

//2位を維持する
int keepSecond(int playerNum, info *array)
{
	int arrayValue = 0;
	char posi[8];
	int tempCard,tempNum,tempMark;
	static int count=15;
	static int upNum;
	static int downNum;
	static int midcount;
	static int incdec;
	if(count==15){//ラウンド毎に初期化
		count=0;
		upNum=1;
		downNum=15;
		midcount=8;
		incdec=1;
	}
	strcpy(posi,array[playerNum].position);
	if(count==0){//初手はランダム
		arrayValue=stratRandom(playerNum, array);
	}else {

		if(strcmp(posi,"max")==0){//1位　最大値と最小値を交互に出す。

			if((downNum - upNum) % 2){
				saveArray[playerNum] = array[playerNum].handsList[upNum];
				sortedArray[playerNum] = array[playerNum].handsList[upNum];
				arrayValue=array[playerNum].handsList[upNum++];
			}else{
				saveArray[playerNum] = array[playerNum].handsList[downNum];
				sortedArray[playerNum] = array[playerNum].handsList[downNum];
				arrayValue=array[playerNum].handsList[downNum--];
			}
		}
		else if(strcmp(posi,"mid")==0 || strcmp(posi,"min")==0){//2位 3位　中央値を出す
			while(1){
				if(midcount>16){
					arrayValue=stratRandom(playerNum,array);
					break;
				}
				tempCard=array[playerNum].handsList[midcount];
				tempNum=tempCard/10;
				tempMark=(tempCard%10)/2;
				if(array[playerNum].hands[tempNum][tempMark]==1){
					arrayValue=tempNum;
					midcount+=incdec;
					if(incdec>0)incdec+=1;
					else incdec-=1;
					incdec*=-1;
					break;
				}else {
					midcount+=incdec;
					if(incdec>0)incdec+=1;
					else incdec-=1;
					incdec*=-1;
				}
			}
		}
	}

	return arrayValue;
}

//手札の最大値・最小値を出す
int ranks_maxOrMin(int num, int playerNum, info *array)
{
	int arrayValue;

	if(num){
		arrayValue = mostMax(playerNum, array);
	}else{
		arrayValue = mostMin(playerNum, array);
	}

	return arrayValue;
}

//各ターン毎の順位に応じて出し方を決定する。　
int ranks(int playerNum, info *array)
{
	int i, j;
	int arrayValue = 0, breakflag = 0;
	int minCount = 0, midCount = 0, maxCount = 0;
	int min = 0, mid = 0, max = 0;
	int tempNum = 0, tempMark = 0;
	int minNum = 0, maxNum = 0, minMark = 0, maxMark = 0;
	static int count = 0;

	//ラウンド毎のターン数をリセット
	if(count == 15){
		count = 0;
	}

	for(i = 0; i < 3; ++i){
		if(strcmp(array[i].position, "min") == 0){
			min = array[i].score;
			++minCount;
			continue;
		}else if(strcmp(array[i].position, "mid") == 0){
			mid = array[i].score;
			++midCount;
			continue;
		}else if(strcmp(array[i].position, "max") == 0){
			max = array[i].score;
			++maxCount;
			continue;
		}
	}

	//前ターンの順位
	switch(array[playerNum].rank){
		case 1:
			if(strcmp(array[playerNum].position, "min") == 0 && minCount >= 1 && midCount == 0 && maxCount >= 1){
				//最小値である
				arrayValue = ((max - min) / 2) * 10;
			}else if(strcmp(array[playerNum].position, "mid") == 0){
				//中間の値である
				if((max - mid) > (mid - min) && minCount >= 1 && midCount >= 1 && maxCount >= 1){
					arrayValue = ((max - min) / 2) * 10;
				}else{
					arrayValue = ranks_maxOrMin(count % 2, playerNum, array);
				}
			}else{
				//最大値である
				arrayValue = ranks_maxOrMin(count % 2, playerNum, array);
			}
			break;
		case 2:
			if((max - mid) >= (mid - min) && minCount >= 1 && midCount >= 1 && maxCount >= 1){
				//最小値である
				arrayValue = ((max + 1) / 2) * 10;
			}else if((max - mid) < (mid - min) && minCount >= 1 && midCount >= 1 && maxCount >= 1){
				//最大値である
				arrayValue = ((max - mid) / 2 + (mid - min)) * 10;
			}else{
				arrayValue = ranks_maxOrMin(count % 2, playerNum, array);
			}
			break;
		case 3:
			if(strcmp(array[playerNum].position, "min") == 0 && minCount >= 1 && midCount >= 1 && maxCount >= 1){
				//最小値である
				arrayValue = (max - mid) / 2 + (mid - min);
			}else{
				arrayValue = ranks_maxOrMin(count % 2, playerNum, array);
			}
			break;
		default:
			arrayValue = rand() % 127 + 12;
			break;
	}

	if(arrayValue < 12){
		arrayValue = 12;
	}
	if(arrayValue > 138){
		arrayValue = 138;
	}

	//arrayValueが奇数->偶数
	if(arrayValue % 2){
		arrayValue += 1;
	}

	//10,20,30等の10の倍数に+2
	if(arrayValue % 10 == 0){
		arrayValue += 2;
	}

	//値付けされた値を復元する
	tempNum = arrayValue / 10;
	tempMark = arrayValue % 10 / 2;
	minNum = arrayValue / 10;
	maxNum = arrayValue / 10;
	minMark= (arrayValue % 10) / 2 - 1;
	maxMark= (arrayValue % 10) / 2 + 1;

	if(array[playerNum].hands[tempNum - 1][tempMark - 1] == 1){
		//arrayValueと同じ手札を出す
		array[playerNum].hands[tempNum -1][tempMark -1] = 0;
		arrayValue = tempNum * 10 + tempMark * 2;
	}else{
		//arrayValudeの付近の手札を探す
		while(1){
			if(minMark < 1){
				--minNum;
				minMark = 4;
			}
			if(maxMark > 4){
				++maxNum;
				maxMark = 1;
			}
			if(array[playerNum].hands[minNum - 1][minMark - 1] && minNum >= 1 && minNum <= 13){
				arrayValue = minNum * 10 + minMark * 2;
				array[playerNum].hands[minNum - 1][minMark - 1] = 0;
				break;
			}
			if(array[playerNum].hands[maxNum - 1][maxMark - 1] && maxNum <= 13 && maxNum >= 1){
				arrayValue = maxNum * 10 + maxMark * 2;
				array[playerNum].hands[maxNum - 1][maxMark - 1] = 0;
				break;
			}

			if(minNum >= 1){
				--minMark;
			}
			if(maxNum <= 13){
				++maxMark;
			}
		}
	}

	++count;
	saveArray[playerNum] = arrayValue;
	sortedArray[playerNum] = arrayValue;

	return arrayValue;
}

/////////////////////////           起動           /////////////////////

void luncher(int maxNum)
{
	int i,j;
	char tempStr[32];
	info *array;

	srand((unsigned)time(NULL));

	for(i = 0; i < maxNum; ++i){
		array = (info*)calloc(3, sizeof(info));
		serverHands1(array);
		handsList(array);	

		//動的メモリを確保できたか否かを判定
		if(array == NULL){
			fprintf(stderr, "Out of memory\n");
			exit(1);	
		}

		puts("---------------------------------------------------------------");
		for(j = 0; j < 15; ++j){
			printf(" [%dターン]\n", j + 1);
			//右オペランドの関数名を変更して戦略を変更する
			printf(" player1: %d\n", putDownCards[j][0] = stratRandom(0, array));
			printf(" player2: %d\n", putDownCards[j][1] = strat123(1, array));
			printf(" player3: %d\n", putDownCards[j][2] = upDown(2, array));

			sortNum();
			chooseWinner(tempStr, array);
			printf(" Win    : %s\n", tempStr);
			nowRank(array);

			printf(" <現在の状況>\n");
			printf(" player1: %d位, player2: %d位, player3: %d位\n", array[0].rank, array[1].rank, array[2].rank);
			printf(" player1: %3d, player2: %3d, player3: %3d\n", array[0].score, array[1].score, array[2].score);
			printf(" player1: %s, player2: %s, player3: %s\n\n", array[0].position, array[1].position, array[2].position);
		}

		printf("%*s[最終結果]\n", 50, "");
		for(j = 0; j < 3; ++j){
			printf("%*splayer%d: sum = %d\n", 50, "", j + 1, array[j].score);
		}

		for(j = 0; j < 3; ++j){
			saveArray[j] = array[j].score;
			sortedArray[j] = array[j].score;
		}
		sortNum();	
		roundWinner(tempStr, array);
		printf("%*sWinner : %s\n\n", 50,"",tempStr);

		memset(tempHandsArray, 0, sizeof(tempHandsArray));
		memset(putDownCards, 0, sizeof(putDownCards));
		free(array);
		array = NULL;
	}

	puts("---------------------------------------------------------------");
	putchar('\n');
	printf(" [勝率]\n");

	for(i = 0; i < 3; ++i){
		printf(" player%d: %d[wins] / (%d[rounds] * 15[turns]) * 100 = %.2f%%\n", i + 1, sumWin[i], maxNum, (double)sumWin[i] / (maxNum * 15) * 100);
	}
	putchar('\n');
	for(i = 0; i < 3; ++i){
		printf(" player%d: %d[wins] / %d[rounds] * 100 = %.2f%%\n", i + 1, countWinRound[i], maxNum, (double)countWinRound[i] / maxNum * 100);
	}
	putchar('\n');

}


int main(void)
{

	int getNum = 0;

	while(getNum < 1){
		printf(" ラウンド回数を入力せよ: ");
		scanf("%d", &getNum);
	}

	luncher(getNum);

	return 0;
}


