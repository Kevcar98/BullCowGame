// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Isograms = GetValidWords(Words);
    SetupGame();

   
}


void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ProcessGame(PlayerInput); 
}
       
void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num()-1)];
    WLength = HiddenWord.Len();
    bGameOver= false;
    Lives=WLength;

    PrintLine(TEXT("Welcome to Bull Cows"));
    PrintLine(TEXT("Number of words are %i"), Words.Num());
    PrintLine(TEXT("The Hidden word is: %s."), *HiddenWord);//This is to debug to know what the word is!
    PrintLine(TEXT("Hello and welcome to Bull Cows!"));
    PrintLine(TEXT("You have %i tries to guess the word!"),Lives);// %i is inserting the integer variable of Lives
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());//%i is inserting the integer variable of WLength
    //PrintLine(TEXT("Guess the four letter word"));// Magic Number Remove!
    PrintLine(TEXT("Guess the word to continue"));    
    


}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    //const TCHAR HW[]=HiddenWord;
    //int32 WLEN = Word.Len()
    for(int32 l=0;l<=Word.Len();l++)
    {
        for(int32 Index=l, Comparison = Index + 1; Comparison < Word.Len();Comparison++)
        {
            if(Word[Index]==Word[Comparison])
            {
                //PrintLine(TEXT("This is not an Isogram! Make sure there are no repeating words!"));
                return false;
            }
        }
    }
    return true;

}




void UBullCowCartridge::EndGame()
{
    bGameOver= true;
    PrintLine(TEXT("Press enter to play again!"));
}




void UBullCowCartridge::ProcessGame(const FString& Guess)
{
    if(bGameOver==true)//if game is over then clear screen and setup game
    {
        ClearScreen();
        SetupGame();
    }
    else
    {           
        //PrintLine(Input);//"Input" stores the input variable of the user and prints it

        if (WLength==Guess.Len())
        {
            if (Guess==HiddenWord)
            {
            PrintLine(TEXT("Congratulations you guessed the word"));
            EndGame();
            }
            else
            {
                PrintLine(TEXT("The Hidden word is: %s."), *HiddenWord);//This is to debug to know what the word is!
                if(!IsIsogram(Guess)){
                    PrintLine(TEXT("The word has repeated words!  It is not an Isogram!"));
                }
                else
                {
                    PrintLine(TEXT("You have %i lives left"),--Lives);
                    if (Lives!=0)
                    {
                        PrintLine(TEXT("Have another guess!"));
                        FBullCowCount Score =GetBullCows(Guess);
                        PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);
                    }
                    if(Lives<=0)
                    {
                        PrintLine(TEXT("You lost!"));
                        PrintLine(TEXT("The word was %s"),*HiddenWord);
                        EndGame();
                    }
                }                        
            }
        }
        else
        {
            PrintLine(TEXT("The word is of size: %i.  Please try again!"),WLength);
        }
    }
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for(FString Word : WordList)
    {
        if(Word.Len()>= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    for(int32 GuessIndex = 0; GuessIndex<Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex]== HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for(int32 HiddenI = 0 ; HiddenI < HiddenWord.Len();HiddenI++)
        {
            if(Guess[GuessIndex]==HiddenWord[HiddenI])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;

}