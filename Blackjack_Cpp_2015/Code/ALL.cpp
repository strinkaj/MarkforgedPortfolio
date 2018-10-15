#include "Header.h"

//Card Constructor
Card::Card(rank r, suit s, bool ifUp) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifUp)
{}

int Card::GetValue() const
{//begin getvalue
	int value = 0;
	if (m_IsFaceUp){//begin if
		value = m_Rank;
		//value is 10 for face cards
		if (value > 10) value = 10;
	}//end if
	return value;
}//end getvalue

void Card::Flip()
{//begin flipcard
	m_IsFaceUp = !(m_IsFaceUp);
}//end flipcard

void Hand::Add(Card* pCard)
{//begin add in hand
	m_Cards.push_back(pCard);
}//end add in hand

void Hand::Clear()
{//begin clear in hand

	vector <Card*>::iterator iter = m_Cards.begin();

	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
	{//begin for
		delete *iter;
		//get rid of dangling pointer
		*iter = 0; 
	}//end for

	m_Cards.clear();
}//end clear in hand

int Hand::GetTotal() const
{//begin gettotal in hand

	//if no cards, return 0
	if (m_Cards.empty()) return 0;

	//if card is face down; return 0
	if (m_Cards[0]->GetValue() == 0) return 0;

	//add up card values (ace as 1)
	int total = 0;

	vector<Card*>::const_iterator iter;

	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		total += (*iter)->GetValue();

	bool containsAce = false;
	for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		if ((*iter)->GetValue() == Card::ACE)
			containsAce = true;

	if (containsAce && total <= 11)
	{ //if there is an ace, add 10
		total += 10;
	} //end if

	return total;
}//end got total in hand

bool GenericPlayer::IsBusted() const
{//begin busted in genplayer
	return (GetTotal() > 21);
}//end busted

void GenericPlayer::Bust() const
{//begin OutBust
	cout << m_Name << " busts.\n";
}//end OutBust

bool Player::IsHitting() const
{//begin is hitting
	cout << m_Name << ", do you want a hit? (y/n): ";
	char response;
	cin >> response;
	return (response == 'y' || response == 'Y');
}//end is hitting

void Player::Win() const
{//begin player win
	cout << m_Name << " wins.\n";
}//end player win

void Player::Lose() const
{//begin player lose
	cout << m_Name << " loses.\n";
}//end player lose

void Player::Push() const
{//begin player tie
	cout << m_Name << " pushes.\n";
}//end player tie

bool House::IsHitting() const
{//begin house hitting
	return (GetTotal() <= 16);
}//end house hitting

void House::FlipFirstCard()
{//begin house flip card
	if (!(m_Cards.empty()))
		m_Cards[0]->Flip();
	else
		cout << "No card to flip!\n";
}//end house flip card

Deck::Deck()
{//begin deck
	m_Cards.reserve(52);
	Populate();
}//end deck

void Deck::Populate()
{//begin populate deck
	Clear();

	//create deck
	for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
	{//begin for
		for (int r = Card::ACE; r <= Card::KING; ++r)
		{//begin for 2
			Add(new Card(static_cast <Card::rank>(r), static_cast <Card::suit>(s)));
		}//end for 2
	}//end for
}//end populate deck

void Deck::Shuffle() //uses algorithm
{//begin shuffle deck
	random_shuffle(m_Cards.begin(), m_Cards.end());
}//end shuffle deck

void Deck::Deal(Hand& aHand)
{//begin deal deck
	if (!m_Cards.empty())
	{//begin if
		aHand.Add(m_Cards.back());
		m_Cards.pop_back();
	}//end if
	else
	{//begin else
		cout << "Out of cards, Unable to deal.\n";
	}//end else
}//end deal deck

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer)
{//begin deck additionalcards
	cout << endl;
	//allow continue to deal a card as long as human/computer player isn't busted
	while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting())
	{//begin while
		Deal(aGenericPlayer);
		cout << aGenericPlayer << endl;
		if (aGenericPlayer.IsBusted())
			aGenericPlayer.Bust();
	}//end while
}//end deck additionalcards

Game::Game(const vector<string>& names)
{//begin game

	vector<string>::const_iterator pName;

	for (pName = names.begin(); pName != names.end(); ++pName)
		m_Players.push_back(Player(*pName));

	srand(time(0));

	m_Deck.Populate();
	m_Deck.Shuffle();
}//end game

void Instructions()
{//begin instructions

	ifstream myReadFile;
	myReadFile.open("Welcome.txt");
	char output[100];
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			cout << output;
		}
	}
	myReadFile.close();

	//Commented out to implement file processing
	//cout << "\tWelcome to Blackjack!\n\n";
	cout << "\n\n\tGoal: Reach 21 without going over\n\n";
	cout << "\tThe House (computer player) will be playing against you\n\n";
	cout << "\tIf the House busts (goes over 21), all players who\n";
	cout << "\thaven't busted will win.\n\n";
	cout << "\tIf a player and the House both get 21\n";
	cout << "\tit is declared a push (tie)\n\n";
	cout << "\tUp to five human players can play\n\n"; 
	
}//end instructions

void Game::Play()
{// begin game play

	vector<Player>::iterator pPlayer;

	for (int i = 0; i < 2; ++i)
	{//begin for
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
			m_Deck.Deal(*pPlayer);
		m_Deck.Deal(m_House);
	}//end for

	//hide house's first card
	m_House.FlipFirstCard();

	//display everyone's hand
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
		cout << *pPlayer << endl;

	cout << m_House << endl;

	//deal additional cards to players
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
		m_Deck.AdditionalCards(*pPlayer);

	//reveal house's first card
	m_House.FlipFirstCard();
	cout << endl << m_House;

	//deal additional cards to house
	m_Deck.AdditionalCards(m_House);

	if (m_House.IsBusted())
	{//begin if
		//everyone still playing wins
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
			if (!(pPlayer->IsBusted()))
				pPlayer->Win();
	}//end if

	else
	{//begin else
		//compare each player still playing to house
		for (pPlayer = m_Players.begin(); pPlayer != m_Players.end();
			++pPlayer)
			if (!(pPlayer->IsBusted()))
			{//begin if
				if (pPlayer->GetTotal() > m_House.GetTotal())
					pPlayer->Win();
				else if (pPlayer->GetTotal() < m_House.GetTotal())
					pPlayer->Lose();
				else
					pPlayer->Push();
			}//end if
	}//end else

	//remove everyone's cards
	for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
		pPlayer->Clear();

	m_House.Clear();
}//end game play

int main()
{//begin main
	Instructions();//display instructions
	
	int numPlayers = 0;

	while (numPlayers < 1 || numPlayers > 5)
	{//begin while
		cout << "How many players? (1 - 5): ";
		cin >> numPlayers;
	}//end while

	vector <string> names;
	string name;

	for (int i = 0; i < numPlayers; ++i)
	{//begin for
		cout << "Enter player name: ";
		cin >> name;
		names.push_back(name);
	}//end for

	cout << endl;

	//game loop
	Game aGame(names);
	char again = 'y';
	while (again != 'n')
	{//begin while
		aGame.Play();
		cout << "\nDo you want to play again? (y/n): ";
		cin >> again;
	}//end while

	//Write to file
	ofstream myfile;
	myfile.open("LastPlayer.txt");
	myfile << name << " played last." << endl;
	myfile.close();
	return 0;
}//end main