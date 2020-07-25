#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#include <boost/random.hpp>
#include <fstream>



class genCircle 	// Generates circle with specified name, radius, centre and colour. 
{	
public:
	sf::CircleShape circleObject;
	std::string _name;
	sf::RenderWindow& _window;

	genCircle(sf::RenderWindow& window, std::string name, float radius, float xCentre, float yCentre, sf::Color colour)
	:_window(window)
	{
		circleObject = sf::CircleShape(radius);
		_name = name;
		circleObject.setPosition(xCentre - radius, yCentre - radius);
		circleObject.setFillColor(colour);
		circleObject.setPointCount(100);
	}

	std::string getName()
	{
		return _name;
	}

	void draw()
	{
		_window.draw(circleObject);
	}
};

class buttonRect	 // Generates rectangular button
{
public:
	sf::Color _color;
	sf::Font& _font;
	float _xPos;
	float _yPos;
	float _width;
	float _height;
	sf::RenderWindow& _window;
	sf::Image image;
	std::string _buttonString;
	sf::RectangleShape _buttonShape;
	sf::RectangleShape _buttonShapeInner;
	sf::Text _buttonText;

	buttonRect(sf::RenderWindow& window, float xPos, float yPos, float width, float height, sf::Color color, std::string buttonString, sf::Font& font)
		
	:_window(window), _font(font)
	{
		_color = color;
		_xPos = xPos;
		_yPos = yPos;
		_width = width;
		_height = height;
		_buttonString = buttonString;


		sf::RectangleShape shape;
		shape.setPosition(sf::Vector2f(xPos, yPos));
		shape.setSize(sf::Vector2f(width, height));
		shape.setFillColor(_color);
		_buttonShape = shape;

		sf::RectangleShape shapeInner;
		int thickness = 10;
		shapeInner.setPosition(sf::Vector2f(xPos + thickness, yPos + thickness));
		shapeInner.setSize(sf::Vector2f(width - 2*thickness, height - 2*thickness));
		sf::Color innerColor = sf::Color::White;
		innerColor.a = 100;
		shapeInner.setFillColor(innerColor);
		_buttonShapeInner = shapeInner;
		
		sf::Text text;
		text.setFont(_font);
		text.setString(_buttonString);
		text.setCharacterSize(40);
		text.setScale(0.66,0.66);

		text.setFillColor(_color);
		sf::FloatRect textRect = text.getLocalBounds();

		text.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);
		text.setPosition(_xPos + _width/2, _yPos + _height/2);

		_buttonText = text;

	}

	bool pressedCheck(float xMouse, float yMouse)
	{
		if ((xMouse >= _xPos) & (xMouse <= _xPos + _width))
		{
			if ((yMouse >= _yPos) & (yMouse <= _yPos + _height))
			{
				sf::Color pressedColor = sf::Color::Black;
				pressedColor.a = 50;
				_buttonShapeInner.setFillColor(pressedColor);
				return true;
			}
		}
		return false;
	}

	bool hoverCheck(float xMouse, float yMouse)
	{
		if ((xMouse >= _xPos) & (xMouse <= _xPos + _width))
		{
			if ((yMouse >= _yPos) & (yMouse <= _yPos + _height))
			{
				return true;
			}
		}
		return false;
	}

	void draw()
	{
		sf::Vector2i position = sf::Mouse::getPosition(_window);
		sf::Vector2f worldPos = _window.mapPixelToCoords(position);
		if ((worldPos.x >= _xPos) & (worldPos.x <= _xPos + _width) & (worldPos.y >= _yPos) & (worldPos.y <= _yPos + _height))
		{
			_buttonText.setFillColor(sf::Color::Yellow); // Hover colour.
		}
		else
		{
			_buttonText.setFillColor(_color);
		}

		_window.draw(_buttonShape);
		_window.draw(_buttonShapeInner);
		_window.draw(_buttonText);
	}
};

class buttonRectSet	// Generates rectangular button set
{
public:
	std::vector<buttonRect> _buttonSet; // Set of buttons created in object.
	std::vector<std::string> _buttonStrings;
	bool _orientation; // False for vertical, true for horizontal
	sf::RenderWindow& _window;
	sf::Font& _font;
	int _quantity;
	float _xPos;
	float _yTop;
	float _buttonWidth;
	float _buttonHeight;
	float _gap;
	sf::Color _buttonColor;

	buttonRectSet(sf::RenderWindow& window, bool orientation, float gap, float xPos, float yTop, float buttonWidth, float buttonHeight, sf::Color buttonColor, std::vector<std::string> buttonStrings, sf::Font& font)
		:_window(window), _font(font)
	{
		_quantity = buttonStrings.size();
		_orientation = orientation;
		_xPos = xPos;
		_yTop = yTop;
		_buttonWidth = buttonWidth;
		_buttonHeight = buttonHeight;
		_buttonColor = buttonColor;
		_gap = gap;
		_buttonStrings = buttonStrings;


		if (_orientation == false)
		{
			for (int i = 0; i < _quantity; i++)
			{
				buttonRect newButton(_window, _xPos, _yTop + i*(_buttonHeight+_gap), _buttonWidth, _buttonHeight, _buttonColor, buttonStrings[i], _font);
				_buttonSet.push_back(newButton);
			}
		}
		else if (_orientation == true)
		{
			if (_quantity % 2 == 1)
			{
				for (int i = 0; i < _quantity; i++)
				{
					buttonRect newButton(_window, _xPos + i * (_buttonWidth + _gap) - (_quantity / 2) * (buttonWidth + _gap), _yTop, _buttonWidth, _buttonHeight, _buttonColor, buttonStrings[i], _font);
					_buttonSet.push_back(newButton);
				}
			}
			else
			{
				for (int i = 0; i < _quantity; i++)
				{
					buttonRect newButton(_window, _xPos + i * (_buttonWidth + _gap) - ((_quantity / 2) - 0.5) * (buttonWidth + _gap), _yTop, _buttonWidth, _buttonHeight, _buttonColor, buttonStrings[i], _font);
					_buttonSet.push_back(newButton);
				}
			}
		}
	}

	void draw()
	{
		for (int i = 0; i < _quantity; i++)
		{
			_buttonSet[i].draw();
		}
	}
};






int init_menu(sf::RenderWindow& window, int screenWidth, int screenHeight, std::vector<int> highScoreList)
{
	sf::Color backgroundColour(0, 17, 33);
	sf::RectangleShape background(sf::Vector2f( screenWidth, screenHeight ));
	background.setFillColor(backgroundColour);

	genCircle titleBackground1(window, "backgroundWhite", 200, screenWidth / 2, screenHeight / 2, sf::Color::White);
	sf::Color grey(72, 70, 70);
	genCircle titleBackground2(window, "backgroundGrey", 400, screenWidth / 2, screenHeight / 2, grey);

	/// Font load
	sf::Font font;
	if (!font.loadFromFile("fonts/FFFFORWA.ttf"))
	{
		// error...
	}

	/// Play button creation
	buttonRectSet playButtons(window, true, 100, screenWidth / 2 - 100, screenHeight / 2 + 350, 200, 100, sf::Color(77, 0, 0), { "Timed", "Accuracy", "Quit" }, font);

	sf::Text titleText1;
	sf::Text titleText2;
	sf::Text highScoreText;

	titleText1.setFont(font);
	titleText1.setString("TIPPY");
	titleText1.setCharacterSize(100);
	titleText1.setFillColor(sf::Color::Red);
	titleText1.setPosition(screenWidth/2 - 200, screenHeight/2 - 100);
	titleText2.setFont(font);
	titleText2.setString("TAP");
	titleText2.setCharacterSize(100);
	titleText2.setFillColor(sf::Color::Red);
	titleText2.setPosition(screenWidth/2 - 65, screenHeight/2 + 40);
	
	highScoreText.setFont(font);
	highScoreText.setString("Highscore: " + std::to_string(highScoreList[0]));
	highScoreText.setCharacterSize(40);
	highScoreText.setFillColor(sf::Color(43, 249, 1));

	/// Centres text origin.
	sf::FloatRect textRect = highScoreText.getLocalBounds();
	highScoreText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	highScoreText.setPosition(screenWidth / 2, screenHeight/2 + 250);


	bool startCheck(false); // Click to start check.

	int effectCounter(0);

	while (window.isOpen())
	{
		// Effect counter check
		if (effectCounter == 40)
		{
			highScoreText.setFillColor(sf::Color(229, 255, 223));
		}
		if (effectCounter == 70)
		{
			highScoreText.setFillColor(sf::Color(43, 249, 1));
			effectCounter = 0;
		}



		sf::Event event;
		
		sf::Vector2i position = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(position);

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)	 // Checks for window close
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) 	// Checks for escape
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
					return -1;
			}
			
			// Checks for menu button pressed, returning a corresponding int to the main() class.
			else if (event.type == sf::Event::MouseButtonPressed & event.mouseButton.button == sf::Mouse::Left)
			{
				for (int i=0; i < playButtons._quantity; i++)
				{
					if (playButtons._buttonSet[i].hoverCheck(worldPos.x, worldPos.y))
					{
						if (i < highScoreList.size()) 
						{
							highScoreText.setString("Highscore: " + std::to_string(highScoreList[i]));
						}
					}

					if (playButtons._buttonSet[i].pressedCheck(worldPos.x, worldPos.y))
					{
						playButtons.draw();

						window.display();
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
						return i;
					}
				}
			}
			// Updates Highscore text on menu with mode.
			for (int i = 0; i < playButtons._quantity - 1; i++)
			{
				if (playButtons._buttonSet[i].hoverCheck(worldPos.x, worldPos.y))
				{
					highScoreText.setString("Highscore: " + std::to_string(highScoreList[i]));
				}
			}
		}

		window.clear();

		window.draw(background);
		window.draw(titleBackground2.circleObject);
		window.draw(titleBackground1.circleObject);
		window.draw(titleText1);
		window.draw(titleText2);
		window.draw(highScoreText);


		playButtons.draw();

		window.display();

		std::this_thread::sleep_for(std::chrono::milliseconds(17)); 	// ~ 60 fps
		
		effectCounter++;
	}
	return 0;
}



int drawScore(sf::RenderWindow& window, int screenWidth, int screenHeight, int score)
{
	sf::Font scoreFont;
	if (!scoreFont.loadFromFile("fonts/unispace bd.ttf"))
	{
		// error...
	}

	sf::Text scoreText;

	scoreText.setFont(scoreFont);
	scoreText.setString("SCORE: " + std::to_string(score));
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(screenWidth/2 - 60, 20);

	window.draw(scoreText);
	return 0;
}

int drawCountdown(sf::RenderWindow& window, int screenWidth, int screenHeight, std::string timeLeft)
{
	sf::Font scoreFont;
	if (!scoreFont.loadFromFile("fonts/unispace bd.ttf"))
	{
		// error...
	}

	sf::Text countdownText;
	sf::Color countdownTextColour(255, 100, 0, 30);
	
	countdownText.setFont(scoreFont);
	countdownText.setString(timeLeft);
	countdownText.setCharacterSize(200);
	countdownText.setFillColor(countdownTextColour);
	
	/// Centres text origin.
	sf::FloatRect textRect = countdownText.getLocalBounds();
	countdownText.setOrigin(textRect.left + textRect.width / 2.0f,
							textRect.top + textRect.height / 2.0f);
	countdownText.setPosition(screenWidth/2, screenHeight/2);

	window.draw(countdownText);
	return 0;
}

// Reads highscores from specified mode file.
int readScore(std::string modeFile)
{
	std::ifstream highScoreRecord(modeFile);
	int highScore;
	highScoreRecord >> highScore;
	highScoreRecord.close();

	return highScore;
}

int recordScore(std::string modeFile, int score, int highScore)
{

	if (score > highScore)
	{
		std::ofstream highScores;
		highScores.open(modeFile);
		highScores << std::to_string(score);
		highScores.close();
	}
	return 0;
}



int init_timedPlayLoop(sf::RenderWindow& window, int screenWidth, int screenHeight, int highScore)
{
	int playerDefaultRadius(5);
	int targetRadius(50);
	sf::Color targetColour(sf::Color::Red);
	sf::Color playerColour(sf::Color::Blue);

	int timeCounterRaw = 31000;	 // Time in milliseconds.

	int boundaryThickness(10); 	// Minimum distance from screen edge targets spawn.
	float targetThickness(5.0f);
	int growthRate(2);

	boost::random::mt19937 rng(time(0));
	boost::random::uniform_int_distribution<> xDist(targetRadius + targetThickness + boundaryThickness, screenWidth - boundaryThickness - targetRadius - targetThickness);
	boost::random::uniform_int_distribution<> yDist(targetRadius + targetThickness + boundaryThickness, screenHeight - boundaryThickness - targetRadius - targetThickness);

	// Generate player and target circles.
	genCircle target(window, "target", targetRadius, screenWidth / 2, screenHeight / 2, targetColour);
	genCircle player(window, "player", playerDefaultRadius, screenWidth / 2, screenHeight / 2, playerColour);
	target.circleObject.setOutlineThickness(targetThickness);
	target.circleObject.setOutlineColor(sf::Color::White);

	// Player position variables. (starts with offscreen values)
	int xPlayer(-100);
	int yPlayer(-100);


	bool growing(false);
	int growingRadius(playerDefaultRadius);

	int points(0);



	typedef std::chrono::high_resolution_clock Clock;
	
	

	while (window.isOpen())
	{
		auto t1 = Clock::now(); // Start time of frame.


		/// Countdown time generation per frame.
		std::string timeCounterString = std::to_string(timeCounterRaw);
		std::string timeCounterDisplay;
		if (timeCounterString.size() < 4) { timeCounterDisplay = "0"; }
		else { timeCounterDisplay = timeCounterString.substr(0, timeCounterString.size() - 3); }

		sf::Vector2i position = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(position);

		auto startTime = std::chrono::system_clock::now();
		sf::Event event;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 	// Grows circle upon holding mouse.
		{
			growingRadius += growthRate;

			if (growing == false)
			{
				xPlayer = worldPos.x - playerDefaultRadius;
				yPlayer = worldPos.y - playerDefaultRadius;
				growing = true;
			}
			else
			{
				xPlayer = worldPos.x - player.circleObject.getRadius();
				yPlayer = worldPos.y - player.circleObject.getRadius();
				growing = true;
			}
		}

		else if (growing) 	// Upon mouse release.
		{
			// Generate random coords for target.
			int xRandom = xDist(rng); 
			int yRandom = yDist(rng);
			

			growing = false;
			growingRadius = playerDefaultRadius;

			int pointsEarned(0);

			sf::Vector2f circleCentre1(target.circleObject.getPosition().x + target.circleObject.getRadius(), target.circleObject.getPosition().y + target.circleObject.getRadius());
			sf::Vector2f circleCentre2(player.circleObject.getPosition().x + player.circleObject.getRadius(), player.circleObject.getPosition().y + player.circleObject.getRadius());

			if (pow((circleCentre1.x - circleCentre2.x), 2) + pow((circleCentre1.y - circleCentre2.y), 2) <= pow(target.circleObject.getRadius() + targetThickness, 2) && pow(pow((circleCentre1.x - circleCentre2.x), 2) + pow((circleCentre1.y - circleCentre2.y), 2), 0.5) + player.circleObject.getRadius() <= target.circleObject.getRadius() + targetThickness)
			{
				pointsEarned = player.circleObject.getRadius() / target.circleObject.getRadius() * 100;
				if (pointsEarned > 100) { pointsEarned = 100; };
				points += pointsEarned/10;
			}

			// Regenerate target and vanish player.
			target.circleObject.setPosition(xRandom - targetRadius, yRandom - targetRadius);
			target.circleObject.setRadius(targetRadius);
			target.circleObject.setOutlineThickness(targetThickness);
			target.circleObject.setOutlineColor(sf::Color::White);
			player.circleObject.setRadius(0);
		}

		if (growingRadius > playerDefaultRadius)
		{
			player.circleObject.setRadius(growingRadius);
		}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
					return 0;
			}
		}

		window.clear();

		player.circleObject.setPosition(xPlayer, yPlayer);


		/// Draws frame.
		sf::Color borderColour(0, 17, 33);
		sf::RectangleShape border(sf::Vector2f(screenWidth, screenHeight));
		border.setFillColor(borderColour);
		sf::RectangleShape background(sf::Vector2f(screenWidth - 2 * boundaryThickness, screenHeight - 2 * boundaryThickness));
		background.setFillColor(sf::Color::Black);
		background.setPosition(boundaryThickness, boundaryThickness);
		window.draw(border);
		window.draw(background);

		drawCountdown(window, screenWidth, screenHeight, timeCounterDisplay);
		window.draw(target.circleObject);
		window.draw(player.circleObject);
		drawScore(window, screenWidth, screenHeight, points);

		window.display();


		timeCounterRaw -= 17;
		if (timeCounterRaw <= 0) { break; }	
		

		/// Removes frametime from sleep between frames.
		auto t2 = Clock::now(); // End time of frame.
		auto timeTaken = t2 - t1;
		auto timeTakenMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken).count();
		std::this_thread::sleep_for(std::chrono::milliseconds(17-timeTakenMilliseconds));
		
		
	}
	recordScore("highScoresTimed.txt", points, highScore);
	std::this_thread::sleep_for(std::chrono::seconds(2)); // Delay return to menu.

	return 0;
}


int init_accuracyPlayLoop(sf::RenderWindow& window, int screenWidth, int screenHeight, int highScore)
{
	int targetRadius(25);
	sf::Color targetColour(sf::Color::Green);
	sf::Color playerColour(sf::Color::Blue);
	

	int boundaryThickness(10); // Minimum distance from screen edge targets spawn.
	int targetThickness(15);

	boost::random::mt19937 rng(time(0));
	boost::random::uniform_int_distribution<> xDist(targetRadius + targetThickness + boundaryThickness, screenWidth - boundaryThickness - targetRadius - targetThickness);
	boost::random::uniform_int_distribution<> yDist(targetRadius + targetThickness + boundaryThickness, screenHeight - boundaryThickness - targetRadius - targetThickness);

	// Generate player and target circles.
	genCircle target(window, "target", targetRadius, screenWidth / 2, screenHeight / 2, targetColour);
	target.circleObject.setOutlineThickness(targetThickness);
	target.circleObject.setOutlineColor(sf::Color::White);

	// Player position variables. (starts with offscreen values)
	int xPlayer(-100);
	int yPlayer(-100);

	typedef std::chrono::high_resolution_clock Clock;


	int points(0); 		// Total points
	int pointsEarned = 5;
	
	int timerBase = 3000; // Initial countdown time.
	int timerResetTime = timerBase;
	int targetsHit = 0;
	int waveCounter = 1;

	int timeCounterRaw = timerBase;		// Countdown time in milliseconds, starts with base time.

	bool endGame = false;

	while (window.isOpen())
	{
		auto t1 = Clock::now(); 	// Start time of frame.

		if (endGame == true) { break; }; 	// Ends game after target is missed.

		/// Countdown time generation per frame.
		std::string timeCounterString = std::to_string(timeCounterRaw);
		std::string timeCounterDisplay;
		if (timeCounterString.size() < 3)
		{
			timeCounterDisplay = "0.0";
		}
		else if (timeCounterString.size() < 4)
		{
			timeCounterDisplay = "0.";
			timeCounterDisplay += timeCounterString.substr(0, 1);
		}
		else 
		{ 
			timeCounterDisplay = timeCounterString.substr(0, 1) +  "." + timeCounterString.substr(1, timeCounterString.size() - 3);
		}

		sf::Vector2i position = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(position);

		auto startTime = std::chrono::system_clock::now();
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
					return 0;
			}
			else if (event.type == sf::Event::MouseButtonPressed & event.mouseButton.button == sf::Mouse::Left)
			{
				int xRandom = xDist(rng);
				int yRandom = yDist(rng);

				if (pow((worldPos.x - (target.circleObject.getPosition().x + targetRadius)), 2) + pow((worldPos.y - (target.circleObject.getPosition().y + targetRadius)), 2) <= pow(targetRadius + targetThickness, 2))
				{
					// Adjusts timer for wave.
					if (targetsHit % 10 == 0)
					{
						waveCounter++;
						timerResetTime = timerBase - waveCounter * (100);
					}

					timeCounterRaw = timerResetTime; 	// Reset timer upon target hit.
					targetsHit++;
					/// Points logic
					points += pointsEarned;

					// Regenerate target.
					target.circleObject.setPosition(xRandom - targetRadius, yRandom - targetRadius);
					target.circleObject.setRadius(targetRadius);
					target.circleObject.setOutlineThickness(targetThickness);
					target.circleObject.setOutlineColor(sf::Color::White);
				}		
				else
				{
					endGame = true;
				}
			}
		
		}

		window.clear();


		/// Draws frame.
		sf::Color borderColour(0, 17, 33);
		sf::RectangleShape border(sf::Vector2f(screenWidth, screenHeight));
		border.setFillColor(borderColour);
		sf::RectangleShape background(sf::Vector2f(screenWidth - 2*boundaryThickness, screenHeight - 2 * boundaryThickness));
		background.setFillColor(sf::Color::Black);
		background.setPosition(boundaryThickness, boundaryThickness);
		window.draw(border);
		window.draw(background);


		drawCountdown(window, screenWidth, screenHeight, timeCounterDisplay);
		window.draw(target.circleObject);
		drawScore(window, screenWidth, screenHeight, points);

		window.display();


		timeCounterRaw -= 17;
		if (timeCounterRaw <= 0) { break; } 	// Ends game upon timer expiry.


		/// Removes frametime from sleep between frames.
		auto t2 = Clock::now(); 	// End time of frame.
		auto timeTaken = t2 - t1;
		auto timeTakenMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeTaken).count();
		std::this_thread::sleep_for(std::chrono::milliseconds(17 - timeTakenMilliseconds));


	}
	recordScore("highScoresAccuracy.txt", points, highScore);
	std::this_thread::sleep_for(std::chrono::seconds(2));	 // Delay return to menu.

	return 0;
}




int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	int screenWidth(1920);
	int screenHeight(1080);

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "TIPPY TAP", sf::Style::Fullscreen, settings);
	
	/// Changes cursor.
	sf::Image cursorImage;
	cursorImage.loadFromFile("cursors/cursor3.png");
	sf::Cursor cursor;
	cursor.loadFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), { 14, 14 });
	window.setMouseCursor(cursor);
	

	// Main gameplay loop.
	while (true)
	{
		std::vector<int> highScoreList;
		int timedHighScore = readScore("highScoresTimed.txt");
		highScoreList.push_back(timedHighScore);
		int accuracyHighScore = readScore("highScoresAccuracy.txt");
		highScoreList.push_back(accuracyHighScore);

		int menu = init_menu(window, screenWidth, screenHeight, highScoreList);
		
		// Reads menu return int to start chosen mode.
		if (menu == -1) { break; }
		else if (menu == 0)
		{
			window.clear();
			init_timedPlayLoop(window, screenWidth, screenHeight, timedHighScore);
		}
		else if (menu == 1)
		{
			window.clear();
			init_accuracyPlayLoop(window, screenWidth, screenHeight, accuracyHighScore);
		}
		else if (menu == 2)
		{
			window.clear();
			break;
		}
		window.clear();
	}
	return 0;
}