#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// --- Start: Custom Liberary ---
void print(string message)
{
  cout << message;
}

void printLine(string message)
{
  print(message);
  cout << '\n';
}

long long getInteger(string message)
{
  print(message);
  long long number;
  cin >> number;

  return number;
}

bool isIntegerBetween(long long number, long long from, long long to)
{
  return (number >= from && number <= to);
}

long long getBoundedInteger(long long from, long long to, string message,
  string errorMessage)
{
  long long number = getInteger(message);

  while(!isIntegerBetween(number, from, to))
  {
    printLine(errorMessage);
    number = getInteger(message);
  }

  return number;
}

unsigned getBoundedRandom(unsigned from, unsigned to)
{
  unsigned rangeSize = (to - from) + 1;
  int random = rand();
  unsigned randomInRange = random % rangeSize;
  unsigned randomValue = from + randomInRange;

  return randomValue;
}

char getChar(string message)
{
  print(message);
  char c;
  cin >> c;

  return c;
}

// --- End: Custom Liberary ---

bool wantToPlayMore()
{
  char wantMore = getChar("Do you want to play more? [Y/N]: ");

  return (wantMore == 'y' || wantMore == 'Y');
}

struct GameResults {
  unsigned short wins = 0, loses = 0, draws = 0; 
};
enum EndGameState {WON, LOSE, DRAW};

string getWinnerName(EndGameState result)
{
  string results[] = {"Player", "Computer", "No Winner"};
  return results[result];
}

string generateUnderscores(unsigned number)
{
  string underscores = "";
  for(int i = 0; i < number; ++i)
    underscores += "_";

  return underscores;
}

EndGameState gameResult(GameResults gameResults)
{
  if(gameResults.wins == gameResults.loses)
    return DRAW;
  
  if(gameResults.wins > gameResults.loses)
    return WON;
    
  return LOSE;
}

void printGameResultBody(GameResults gameResults)
{
  EndGameState result = gameResult(gameResults);

  printLine("\t\t"+generateUnderscores(20) + " [Game Results] " 
    + generateUnderscores(20) + "\t\t");
  printLine("");

  unsigned short rounds = gameResults.wins + gameResults.loses 
    + gameResults.draws;
  printLine("\t\tGame Rounds \t\t: " + to_string(rounds));
  printLine("\t\tPlayer won times\t: " + to_string(gameResults.wins));
  printLine("\t\tComputer won times\t: " + to_string(gameResults.loses));
  printLine("\t\tDraw times \t\t: " + to_string(gameResults.draws));
  printLine("\t\tFinal Winner \t\t: " + getWinnerName(result));
  
  printLine("\t\t" + generateUnderscores(56) + "\t\t");
}

void printGameResultHeader()
{
  printLine("\t\t" + generateUnderscores(56) + "\t\t");
  printLine("");
  printLine("\t\t\t\t+++ G a m e   O v e r +++\t\t\t\t");
  printLine("\t\t" + generateUnderscores(56) + "\t\t");
}

void affects(EndGameState result)
{
  switch(result)
  {
    case WON:
      print("\033[42m\033[2J\033[H");
      break;
    case LOSE:
      print("\a\033[41m\033[2J\033[H");
      break;
    case DRAW:
      print("\033[43m\033[2J\033[H");
      break;
  }
}

void printGameResult(GameResults gameResults)
{
  EndGameState result = gameResult(gameResults);
  affects(result);

  printLine("");
  printGameResultHeader();
  printLine("");
  printGameResultBody(gameResults);
  printLine("");
}

GameResults updateGameResults(GameResults gameResults, EndGameState result)
{
  switch(result)
  {
    case WON:
      gameResults.wins++;
      break;
    case LOSE:
      gameResults.loses++;
      break;
    case DRAW:
      gameResults.draws++;
      break;
  }

  return gameResults;
}

enum Choice {STONE = 1, PAPER = 2, SCISSOR = 3};
string getChoiceName(Choice choice)
{
  string choices[] = {"Stone", "Paper", "Scissor"};
  return choices[choice - 1];
}

EndGameState roundResult(Choice playerChoice, Choice computerChoice)
{
  if(playerChoice == computerChoice)
    return DRAW;
  
  short diff = playerChoice - computerChoice;
  if((diff == 1) || (diff == -2))
    return WON;
    
  return LOSE;
}

void printRoundResult(unsigned short roundNumber, Choice playerChoice, 
  Choice computerChoice)
{
  EndGameState result = roundResult(playerChoice, computerChoice);
  
  affects(result);

  printLine("");
  printLine("__________ Round [" + to_string(roundNumber) + "] __________");
  printLine("");
  printLine(" Player   Choice: " + getChoiceName(playerChoice));
  printLine(" Computer Choice: " + getChoiceName(computerChoice));
  printLine(" Round Winner   : [" + getWinnerName(result) + "]");
  printLine("_______________________________");
}

Choice getChoice(unsigned short choice)
{
  return (Choice) choice;
}

EndGameState playRound(unsigned short roundNumber)
{
  printLine("\nRound [" + to_string(roundNumber) + "] begins:\n");

  unsigned choice = getBoundedInteger(1, 3, 
    "Your Choice: [1]:Stone, [2]:Paper [3]:Scissor ? ", 
    "Wrong input, Please try again.");

  Choice playerChoice = getChoice(choice);
  Choice computerChoice = getChoice(getBoundedRandom(1, 3)); 

  printRoundResult(roundNumber, playerChoice, computerChoice);

  return roundResult(playerChoice, computerChoice);
}

void playGame(unsigned short rounds)
{
  GameResults gameResults;

  for(unsigned short i = 0; i < rounds; ++i)
  {
    EndGameState result = playRound(i+1);
    gameResults = updateGameResults(gameResults, result);
    this_thread::sleep_for(chrono::seconds(1));
  }

  printGameResult(gameResults);
}

void setDefaultTerminal()
{
  print("\033[49m\033[2J\033[H");
}

void game()
{
  bool playMore = true;

  while(playMore)
  {
    setDefaultTerminal();
    unsigned rounds = getBoundedInteger(1, 10, "How many rounds 1 To 10: ",
    "Wrong input, Please try again.");
    
    playGame(rounds);

    playMore = wantToPlayMore();
  }

  setDefaultTerminal();
}

int main()
{
  srand(time(nullptr)); 

  game();
}