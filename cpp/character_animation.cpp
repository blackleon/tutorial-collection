#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

const int width = 120; //number of horizontal characters - change to fit in your console
const int playerWidth = 5;
int playerPos = 0;
int playerDir = 2; //0 for left, 1 for idle, 2 for right
bool walking = false;
bool stepFrame = false; //to switch between foot frames
int framesBeforeForward = 6; //buffer frames to hide 2nd char delay
int framesPerSecond = 30; //frames per second (30 is ideal)

//character visuals
string playerHead[] = 		{" ||| ", " \\|/ ",  "|||  "};
string playerFace[]=		{"< o| ",  "(ovo)",  "|o > "};
string playerChest[] =		{" (!| ",  "/|*|\\", "|!)  "};
string playerFeet[] =		{" dd  ",  " d b ",  " bb  "};
string playerFeetStep[] =	{"d  d ",  " d b ", "b  b "};

void ClearScreen() //custom clear screen
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

string GetFullLine() //get a line of '#'
{
	string text = "";
	for(int i = 0; i < width; i++)
	{
		text += '#';
	}
	text += "\n";
	return text;
}

string GetSpaces(int count) //get x number of spaces
{
	string spaces = "";
	for(int i = 0; i < count; i++)
	{
		spaces += " ";
	}
	
	return spaces;
}

string GetLineWithPart(string part) //get line with character part
{
	string text = "";
	text += "#";
	text += GetSpaces(playerPos);
	text += part;
	text += GetSpaces(width - playerPos - playerWidth - 2);
	text += "#\n";
	
	return text;
}

string GetMiddleLine(int index) //get line for specific index
{
	string text = "";
	switch(index)
	{
		case 1:
		{
			text += GetLineWithPart(playerHead[playerDir]);
			break;
		}
		case 2:
		{
			text += GetLineWithPart(playerFace[playerDir]);
			break;
		}
		case 3:
		{	
			text += GetLineWithPart(playerChest[playerDir]);
			break;
		}
		case 4:
		{
			if(stepFrame)
			{
				text += GetLineWithPart(playerFeetStep[playerDir]);
			}else{
				text += GetLineWithPart(playerFeet[playerDir]);
			}
			stepFrame = !stepFrame;
			break;
		}
			
	}
	
	return text;
}

string GetScreen() //get entire screen string
{
	string text = "";

	text += GetFullLine();
	text += GetMiddleLine(1);
	text += GetMiddleLine(2);
	text += GetMiddleLine(3);
	text += GetMiddleLine(4);
	text += GetFullLine();

	return text;
}

int main() //main flow
{
	int delay = 1.0 / framesPerSecond * 1000;
	int frameCount;
	
	while(true)
	{
		int key = 0;
		if(kbhit()) //if key pressed
		{
			frameCount = 0;;
			key = getch(); //get key
			if(key == 'a') //if key is a move left
			{
				walking = true;
				playerDir = 0;
				playerPos -= 1;
				if(playerPos < 0)
				{
					playerPos = 0;
				}
			}
			else if(key == 'd') //if key is d move right
			{
				walking = true;
				playerDir = 2;
				playerPos += 1;
				if(playerPos > width - playerWidth - 2)
				{
					playerPos = width - playerWidth - 2;
				}
			}else if(key == 's') //if key is s terminate
			{
				break;
			}
		}
		
		if(key == 0) //if no key is pressed for a while idle
		{
			walking = false;
			frameCount++;
			if(frameCount >= framesBeforeForward)
			{
				walking = false;
				playerDir = 1;
				frameCount = 0;
			}
		}
		
		ClearScreen(); //clearscreen
		cout<<GetScreen(); //draw new screen
		this_thread::sleep_for(chrono::milliseconds(delay)); //delay (to fit framerate)
	}
	
	return 0;
}
