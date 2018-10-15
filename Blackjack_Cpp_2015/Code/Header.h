#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;

class Card{//begin class card;
public:
	enum rank {//begin enum rank
		ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
		JACK, QUEEN, KING
	};//end enum rank
	enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	//overloading  << operator
	friend ostream& operator<<(ostream& os, const Card& aCard);
	Card(rank r = ACE, suit s = SPADES, bool ifUp = true);
	int GetValue() const;
	void Flip();

private:
	rank m_Rank;
	suit m_Suit;
	bool m_IsFaceUp;
};//end class card

class Hand{//begin class hand
public:
	Hand();//constructor
	virtual ~Hand();//destructor

	//adds a card to the hand
	void Add(Card* pCard);

	//clears hand
	void Clear();

	//gets total value of hand
	int GetTotal() const;

protected:
	vector <Card*> m_Cards;
};//end class hand

//Hand Constructor/Destructor
Hand::Hand()
{//begin constructor
	m_Cards.reserve(7);
}//end constructor
Hand::~Hand()
{//begin destructor
	Clear();
}//end destructor

//abstract class (base for player and house classes)
class GenericPlayer : public Hand
{//begin genplayer
	friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);
public:
	GenericPlayer(const string& name = "");
	virtual ~GenericPlayer();
	virtual bool IsHitting() const = 0; //pure virtual function
	bool IsBusted() const;
	void Bust() const;

protected:
	string m_Name;
};//end genplayer

GenericPlayer::GenericPlayer(const string& name) : m_Name(name) {}
GenericPlayer::~GenericPlayer() {}

class Player : public GenericPlayer //Player is a GenPlayer
{//begin player
public:
	Player(const string& name = "");
	virtual ~Player();
	virtual bool IsHitting() const;
	void Win() const;
	void Lose() const;
	void Push() const;
};//end player

Player::Player(const string& name) : GenericPlayer(name) {}//Player constructor
Player::~Player() {}//Player Destructor

class House : public GenericPlayer //house is a Genplayer
{//begin house
public:
	House(const string& name = "House");
	virtual ~House();
	//house will always hit on 10 or less
	virtual bool IsHitting() const;
	void FlipFirstCard();
};//end house

//House Constructor/Destructors
House::House(const string& name) : GenericPlayer(name) {}
House::~House() {}

class Deck : public Hand //deck is a hand (collection of cards)
{//begin deck
public:
	Deck();
	virtual ~Deck();
	//populate creates deck of 52 cards
	void Populate();
	void Shuffle();
	void Deal(Hand& aHand);
	void AdditionalCards(GenericPlayer& aGenericPlayer);
};//end deck

//Deck Destructor
Deck::~Deck() {}

class Game
{//begin game
public:
	Game(const vector<string>& names);
	~Game();
	void Play();
	friend void Instructions();

private:
	Deck m_Deck;
	House m_House;
	vector<Player> m_Players;
};//end game

//Game Destructor
Game::~Game() {}

//overloaded function prototypes
ostream& operator<<(ostream& os, const Card& aCard);
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);

//overloads << operator
ostream& operator<<(ostream& os, const Card& aCard)
{//begin << 
	//rank 0 is for facedown card
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
		"10", "J", "Q", "K" };
	const string SUITS[] = { " of clubs", " of diamonds", " of hearts", " of spades" };
	if (aCard.m_IsFaceUp)
		os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
	else
		os << "XX";
	return os;
}//end <<

//overloads << operator
ostream& operator <<(ostream& os, const GenericPlayer& aGenericPlayer)
{//begin overload <<
	os << aGenericPlayer.m_Name << ":\t";
	vector <Card*>::const_iterator pCard;
	if (!aGenericPlayer.m_Cards.empty())
	{//begin if
		for (pCard = aGenericPlayer.m_Cards.begin();
			pCard != aGenericPlayer.m_Cards.end(); ++pCard)
			os << *(*pCard) << "\t";

		if (aGenericPlayer.GetTotal() != 0)
			cout << "(" << aGenericPlayer.GetTotal() << ")";
	}//end if
	else
		os << "<empty>";
	return os;
}// end overload <<