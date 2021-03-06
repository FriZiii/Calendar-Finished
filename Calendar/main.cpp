#include <SFML/Graphics.hpp>

#include "SidePanel.h"
#include "Background.h"
#include "DaysOfTheWeek.h"
#include "DaysBoxes.h"
#include "Button.h"
#include "Logo.h"
#include "MonthAndYear.h"
#include "Functions.h"
#include "RightBarText.h"
#include "ColorPalettes.h"
#include "Text.h"
#include "ScrollButton.h"
#include "CreateYourOwnColorButton.h"
#include "TextInputDrawing.h"
#include "InputOutputManager.h"
#include <vector>
#include <time.h>
#include <fstream>
#include <string>

#pragma warning(disable : 4996)
int WinMain()
{
	//Antialiasing
	sf::ContextSettings contextsettings;
	contextsettings.antialiasingLevel = 8;

	//Window
	sf::RenderWindow window(sf::VideoMode(1200, 610), "Calendar", 4U, contextsettings);
	window.setFramerateLimit(60);

	//Icon of window
	sf::Image windowIcon;
	windowIcon.loadFromFile("Assets/Icon/Icon_256x256.png");
	window.setIcon(256, 256, windowIcon.getPixelsPtr());

	//Startup value
	bool home = true;
	bool credits = false;
	bool settings = false;

	bool show_Event = false;
	bool submit_eventORcancel_Event = false;

	int marks = 0;
	int total_marks = 0;
	int marks_in_line[11]{};
	int line = 0;

	bool isClicked = false;

	std::string inputText{};
	sf::Clock clockCursor;

	std::fstream file;
	file.open("Startup/settings.txt", std::ios::in);
	int R{};
	int G{};
	int B{};

	if (file.good() == true)
	{
		file >> R;
		file >> G;
		file >> B;
	}
	file.close();
	sf::Color maincolor(R, G, B);

	//Fonts
	sf::Font font; font.loadFromFile("Fonts/WILD_WORLD.otf");
	sf::Font Roboto_Mono_Regular; Roboto_Mono_Regular.loadFromFile("Fonts/RobotoMono-Light.ttf");
	sf::Font Roboto_Mono_Bold; Roboto_Mono_Bold.loadFromFile("Fonts/RobotoMono-Bold.ttf");

	//Background
	Background background;

	//Left and Right bar
	SidePanel left(sf::Vector2f(266.0f, 610.0f), sf::Vector2f(0.0f, 0.0f), maincolor);
	SidePanel right(sf::Vector2f(286.0f, 87.0f), sf::Vector2f(914.0f, 0.0f), maincolor);

	//Logo
	Logo logo(sf::Vector2f(133.0f, 45.0f), sf::Vector2f(105, 105), "Assets/Logo.png");

	//Buttons left bar
	std::vector<Button>button;
	button.push_back(Button(sf::Vector2f(20.0f, 190.0f), font, "HOME", false, maincolor));
	button.push_back(Button(sf::Vector2f(20.0f, 290.0f), font, "SETTINGS", false, maincolor));
	button.push_back(Button(sf::Vector2f(20.0f, 390.0f), font, "CREDITS", false, maincolor));
	button.push_back(Button(sf::Vector2f(20.0f, 490.0f), font, "EXIT", false, maincolor));

	//Month and Year
		//Getting actual month and year an day
	time_t theTime = time(NULL);
	struct tm tm = *localtime(&theTime);
	int actualmonth = tm.tm_mon + 1;
	int actualyear = tm.tm_year + 1900;
	int actualday = tm.tm_mday;
	MonthAndYear monthAndYear(actualmonth, actualyear, font, maincolor);

	//Days of the week
	std::vector<DaysOfTheWeek>daysoftheweek;

	for (int i = 0; i <= 6; i++)
	{
		sf::Vector2f position(293.0f + (float)i * (68.0f + 20.0f), 121.0f);
		daysoftheweek.push_back(DaysOfTheWeek(i, position, font));
	}

	//Boxes of the day
	std::vector<DaysBoxes>daysboxes;

	int daysCount = 0;
	int weekDay = zellerArgorithm(monthAndYear.GetMonth(), monthAndYear.GetYear()); //How many days we have to skip
	for (int i = 0; i <= 5; i++)
	{
		for (int j = 0; j <= 6; j++)
		{
			if (daysCount < DaysCount(monthAndYear.GetMonth(), monthAndYear.GetYear()))//If the current day is less than the number of days in the month, we increase the next day
			{
				daysCount++;
			}
			else
			{
				daysCount = 50;//otherwise, we set a placeholder value
			}

			sf::Vector2f position(293.0f + (float)j * (68.0f + 20.0f), 151.0f + (float)i * 72.0f);
			daysboxes.push_back(DaysBoxes(position, font, daysCount, weekDay));
			weekDay--;
		}
	}

	//Right bar text;
	int day = -1;
	RightBarText rightbartext(font);

	//ColorPallets
	Text colorpalettesText("PICK YOUR ", "COLOR ", "", sf::Vector2f(455.0f, 75.0f), font, maincolor, 40);
	Text scrollbuttonText("or create your own", sf::Vector2f(465.0f, 230.0f), font, 40);

	std::vector<ColorPalettes> colorpalettes;
	int k = 0;
	for (int i = 0; i < 10; i++)
	{
		sf::Vector2f position(344.0f + 30.0f + ((float)i * 80.0f), 175.0f);
		colorpalettes.push_back(ColorPalettes(position, k));
		k++;
	}

	//ScrollButtons
	ScrollButton scrollR(sf::Vector2f(344.0f, 320));
	ScrollButton scrollG(sf::Vector2f(344.0f, 370));
	ScrollButton scrollB(sf::Vector2f(344.0f, 420));

	CreateYourOwnColorButton buttoncolor(sf::Vector2f(1000, 380));

	//Credits
	Logo antArmy(sf::Vector2f(1075.0f, 40.0f), sf::Vector2f(122, 111), "Assets/Ant_Army.png");
	std::vector<Text> creditsText;
	creditsText.push_back(Text("Ant Army Productions", sf::Vector2f(355, 70), font, 43));
	creditsText.push_back(Text("Project ", "leader ", "Mateusz Sawosz", sf::Vector2f(350, 170), font, maincolor, 33));
	creditsText.push_back(Text("Project ", "Assistant Madzia Szmatloch", sf::Vector2f(295, 230), font, maincolor, 33));
	creditsText.push_back(Text("UI/UX ", "Designer Mateusz Sawosz", sf::Vector2f(340, 290), font, maincolor, 33));
	creditsText.push_back(Text("Programmer ", "Madzia Szmatloch", sf::Vector2f(370, 350), font, maincolor, 33));
	creditsText.push_back(Text("Programmer ", "Jakub Pietrasik", sf::Vector2f(410, 410), font, maincolor, 33));
	creditsText.push_back(Text("Software ", "engineer ", "Mateusz Sawosz", sf::Vector2f(310, 470), font, maincolor, 33));

	//Event 
	TextInputDrawing textinputDrawing(Roboto_Mono_Regular, inputText);

	Button addevent(sf::Vector2f(945.0f, 500.0f), font, "ADD EVENT", true, maincolor);
	Button submit_event(sf::Vector2f(945.0f, 500.0f), font, "SUBMIT", true, maincolor);
	Button delete_event(sf::Vector2f(945.0f, 500.0f), font, "DELETE", true, maincolor);
	Button cancel_event(sf::Vector2f(945.0f, 500.0f), font, "CANCEL", true, maincolor);

	SidePanel eventBackground(sf::Vector2f(286.0f, 367.0f), sf::Vector2f(914.0f, 130.0f), sf::Color(232, 232, 232));
	InputOutputManager output_manager;
	InputOutputManager input_manager;
	InputOutputManager remove_manager;

	//Main loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Close and clicking events
			if (event.type == sf::Event::Closed)
			{
				std::ofstream ofs;
				ofs.open("Startup/settings.txt", std::ofstream::out | std::ofstream::trunc);
				ofs.close();

				file.open("Startup/settings.txt", std::ios::out);

				if (file.good() == true)
				{
					file << R << ' ' << G << ' ' << B << ' ';
				}
				window.close();
			}
			else if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				isClicked = true;
			}
			else
			{
				isClicked = false;
			}

			//Text input event
			if (submit_eventORcancel_Event)
			{
				if (inputText == "")
				{
					marks = 0;
					total_marks = 0;
				}
				if (event.type == sf::Event::TextEntered && total_marks <= 208)
				{
					if (std::isprint(event.text.unicode))
					{
						if (marks >= 19)
						{
							inputText += '\n';
							line++;
							marks = 0;
						}
						inputText += event.text.unicode;
						marks++;
						total_marks++;
						marks_in_line[line] = marks;
					}
				}
				else if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::BackSpace)
					{
						if (!inputText.empty())
						{
							if (marks == 0)
							{
								line--;
								marks = marks_in_line[line];
								total_marks -= (19 - marks_in_line[line]);
							}
							else
							{
								marks--;
								total_marks--;
								marks_in_line[line]--;
							}
							inputText.pop_back();
						}
					}
					if (event.key.code == sf::Keyboard::Enter && line < 10)
					{
						marks_in_line[line] = marks;
						line++;
						inputText += '\n';
						total_marks += (19 - marks);
						marks = 0;
					}
				}
			}
		}

		//Updates
		for (Button& button : button)
			button.Update(window, maincolor);

		left.Update(maincolor);

		if (button[0].IsHover(window) && isClicked)
		{
			home = true;
			settings = false;
			credits = false;
		}
		else if (button[1].IsHover(window) && isClicked)
		{
			home = false;
			settings = true;
			credits = false;
		}
		else if (button[2].IsHover(window) && isClicked)
		{
			home = false;
			settings = false;
			credits = true;
		}
		else if (button[3].IsHover(window) && isClicked)
		{
			std::ofstream ofs;
			ofs.open("Startup/settings.txt", std::ofstream::out | std::ofstream::trunc);
			ofs.close();

			file.open("Startup/settings.txt", std::ios::out);
			if (file.good() == true)
			{
				file << R << ' ' << G << ' ' << B << ' ';
			}
			window.close();
		}

		// After clicking remove event
		if (delete_event.IsHover(window) && isClicked && show_Event)
		{
			std::string fileNameString = remove_manager.GetFileName(monthAndYear.GetYear(), monthAndYear.GetMonth(), day);
			int lengthofFileName = (int)fileNameString.length();
			char* fileName = new char[lengthofFileName + 1];
			//Convert string to char
			strcpy(fileName, fileNameString.c_str());

			remove(fileName);
			inputText = "";
			total_marks = 0;
			marks = 0;
			for (int i = 0; i < 11; i++)
				marks_in_line[i] = 0;
			line = 0;
			delete[] fileName;
		}
		// After clicking stop showing the add event button and stat showing submit_event button
		else if (addevent.IsHover(window) && isClicked && !submit_eventORcancel_Event)
		{
			submit_eventORcancel_Event = true;
			isClicked = false;
		}
		//After clicking save the text to file delete_event the text and stop showing the submit_event button
		else if (submit_event.IsHover(window) && isClicked && submit_eventORcancel_Event && day != -1 || cancel_event.IsHover(window) && isClicked && submit_eventORcancel_Event && day != -1)
		{
			if (inputText != "")
				input_manager.SaveEventToFile(monthAndYear.GetYear(), monthAndYear.GetMonth(), day, inputText);

			inputText = "";
			total_marks = 0;
			marks = 0;
			for (int i = 0; i < 11; i++)
				marks_in_line[i] = 0;
			line = 0;
			submit_eventORcancel_Event = false;
			isClicked = false;
		}


		if (home && !credits && !settings)
		{
			right.Update(maincolor);
			monthAndYear.HoverEffect(window, maincolor);
			monthAndYear.ColorUpdate(maincolor);
			if (isClicked && (monthAndYear.leftIsHover(window) || monthAndYear.rightIsHover(window)))
			{
				monthAndYear.Update(window, maincolor);
				day = -1;
				isClicked = false;
			}

			int daysCount = 0;
			int weekDay = zellerArgorithm(monthAndYear.GetMonth(), monthAndYear.GetYear());//How many days we have to skip
			for (DaysBoxes& daysboxes : daysboxes)
			{
				if (weekDay <= 0)
				{
					if (daysCount < DaysCount(monthAndYear.GetMonth(), monthAndYear.GetYear()))//If the current day is less than the number of days in the month, we increase the next day
						daysCount++;
					else
						daysCount = 50;//otherwise, we set a placeholder value
				}
				daysboxes.Update(window, daysCount, weekDay, maincolor, R, G, B, actualday, actualmonth, monthAndYear.GetMonth(), actualyear, monthAndYear.GetYear());
				weekDay--;

				if (daysboxes.isClick(window))
				{
					day = daysboxes.GetDay(window);
				}
			}
			InputOutputManager input_output_manager;
			if (input_output_manager.foundFile(monthAndYear.GetYear(), monthAndYear.GetMonth(), day))
			{
				show_Event = true;
			}
			else
			{
				show_Event = false;
			}

			rightbartext.Update(day, monthAndYear.GetMonthString(), right.GetSize(), right.GetPosition(), monthAndYear.GetMonthString_before(), monthAndYear.GetMonthString_after());

			if (!show_Event && !submit_eventORcancel_Event && day != -1)
			{
				addevent.Update(window, maincolor);
			}

			if (submit_eventORcancel_Event && day != -1)
			{
				if (inputText == "")
					cancel_event.Update(window, maincolor);
				else
					submit_event.Update(window, maincolor);

				textinputDrawing.Update(inputText, clockCursor, total_marks);
			}

			if (show_Event)
			{
				delete_event.Update(window, maincolor);
			}
		}
		if (!home && !credits && settings)
		{
			for (ColorPalettes& colorpalettes : colorpalettes)
			{
				colorpalettes.Update(window);

				if (isClicked && colorpalettes.isHover(window))
				{
					R = colorpalettes.getColorR(window);
					G = colorpalettes.getColorG(window);
					B = colorpalettes.getColorB(window);
				}
			}

			scrollR.Update(window, "R");
			scrollG.Update(window, "G");
			scrollB.Update(window, "B");

			if (buttoncolor.isHover(window) && isClicked)
			{
				R = scrollR.GetIterator();
				G = scrollG.GetIterator();
				B = scrollB.GetIterator();
			}

			maincolor = sf::Color(R, G, B);
			sf::Color ownColor = sf::Color(scrollR.GetIterator(), scrollG.GetIterator(), scrollB.GetIterator());
			buttoncolor.Update(window, ownColor);
			colorpalettesText.Update(maincolor);
		}
		if (!home && credits && !settings)
		{
			for (Text& creditsText : creditsText)
				creditsText.Update(maincolor);
		}
		//Draw
		window.clear(sf::Color::White);

		left.Draw(window);
		logo.Draw(window);

		for (Button& button : button)
			button.Draw(window);

		if (home && !credits && !settings)
		{
			background.Draw(window);

			right.Draw(window);

			for (DaysOfTheWeek& daysoftheweek : daysoftheweek)
				daysoftheweek.Draw(window);

			for (DaysBoxes& daysboxes : daysboxes)
				daysboxes.Draw(window);

			monthAndYear.Draw(window);

			rightbartext.Draw(window);

			//Show the event that you found in the file
			if (show_Event)
			{
				eventBackground.Draw(window);
				delete_event.Draw(window);
				output_manager.DrawTextFromFile(monthAndYear.GetYear(), monthAndYear.GetMonth(), day, Roboto_Mono_Bold, window, maincolor);
			}
			// If you have not found the event file, show the option of adding an event
			else if (!show_Event && !submit_eventORcancel_Event && day != -1)
			{
				addevent.Draw(window);
			}
			//If you have entered an event, save it to a file
			else if (submit_eventORcancel_Event && day != -1)
			{
				eventBackground.Draw(window);
				if (inputText == "")
					cancel_event.Draw(window);
				else
					submit_event.Draw(window);

				textinputDrawing.Draw(window);
			}

		}
		if (!home && !credits && settings)
		{
			colorpalettesText.Draw(window);

			for (ColorPalettes& colorpalettes : colorpalettes)
				colorpalettes.Draw(window);

			scrollbuttonText.Draw(window);

			scrollR.Draw(window);
			scrollG.Draw(window);
			scrollB.Draw(window);
			buttoncolor.Draw(window);
		}
		if (!home && credits && !settings)
		{
			antArmy.Draw(window);
			for (Text& creditsText : creditsText)
				creditsText.Draw(window);
		}

		window.display();
	}

	return 0;
}