#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	_paused = false;
	_pKeyDown = false;
	_start = true;
	_SKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0; 
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0; 

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	// Set Menu Paramters

	_menuBackground = new Texture2D();

	_menuBackground->Load("Textures/Transparency.png", false);

	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	_pacmanCurrentFrameTime += elapsedTime;
	
	if (!_start)
	{
		CheckPaused(keyboardState, Input::Keys::P);
		if (!_paused)
		{
			Input(elapsedTime, keyboardState);

		}
	}
	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && !_SKeyDown)
	{
		_SKeyDown = true;
		_start = !_start;
	}

}
void Pacman::UpdatePacman(int elapsedTime)
{

	if (_pacmanPosition->X + _pacmanSourceRect->Width > 1024)
	{
		_pacmanPosition->X = -30 + _pacmanSourceRect->Width;
	}
	else if (_pacmanPosition->X + _pacmanSourceRect->Width < 25)
	{
		_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
	}
	else if (_pacmanPosition->Y + _pacmanSourceRect->Height > 770)
	{
		_pacmanPosition->Y = -30 + _pacmanSourceRect->Height;
	}
	else if (_pacmanPosition->Y + _pacmanSourceRect->Height < 30)
	{
		_pacmanPosition->Y = 770 - _pacmanSourceRect->Height;
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	_munchieCurrentFrameTime += elapsedTime;
	if (_munchieCurrentFrameTime > _cMunchieFrameTime)
	{
		_munchieFrameCount++;

		if (_munchieFrameCount >= 2)
			_munchieFrameCount = 0;

		_munchieCurrentFrameTime = 0;
	}
}

void Pacman::CheckViewportCollision()
{

}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacmanDirection = 0;
		_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;
			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;
			_pacmanCurrentFrameTime = 0;
			_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
		}
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacmanDirection = 2;
		_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;
			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;
			_pacmanCurrentFrameTime = 0;
			_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
		}
	}
	// Checks if W key is pressed
	else	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
		_pacmanDirection = 3;
		_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;
			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;
			_pacmanCurrentFrameTime = 0;
			_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
		}
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
		_pacmanDirection = 1;
		_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;
			if (_pacmanFrame >= 2)
				_pacmanFrame = 0;
			_pacmanCurrentFrameTime = 0;
			_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
		}
	}
}
void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_munchieFrameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_munchieFrameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_munchieFrameCount++;

		if (_munchieFrameCount >= 60)
			_munchieFrameCount = 0;
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (_paused)

	{

		std::stringstream menuStream; menuStream << "PAUSED!";
	SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
	SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
}
	if (_start)

	{

		std::stringstream menuStream; menuStream << "Press Space to Start the Game";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}