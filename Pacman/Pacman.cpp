#include "Pacman.h"
#include <sstream>
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	_munchie = new Enemy();
	_pacman = new Player();
	_paused = false;
	_pKeyDown = false;
	_start = true;
	_SKeyDown = false;
	_munchieCurrentFrameTime = 0; 
	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->currentFrameTime = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _munchie->BlueTexture;
	delete _munchie->InvertedTexture;
	delete _munchie->Rect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchie->BlueTexture = new Texture2D();
	_munchie->BlueTexture->Load("Textures/Munchie.tga", true);
	_munchie->InvertedTexture = new Texture2D();
	_munchie->InvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchie->Rect = new Rect(100.0f, 450.0f, 12, 12);

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
	_pacman->currentFrameTime += elapsedTime;
	
	if (!_start)
	{
		CheckPaused(keyboardState, Input::Keys::P);
		if (!_paused)
		{
			Input(elapsedTime, keyboardState);
			UpdatePacman(elapsedTime);
			UpdateMunchie(elapsedTime);
			CheckViewportCollision();
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

	if (_pacman->position->X + _pacman->sourceRect->Width > 1024)
	{
		_pacman->position->X = -30 + _pacman->sourceRect->Width;
	}
	else if (_pacman->position->X + _pacman->sourceRect->Width < 25)
	{
		_pacman->position->X = 1024 - _pacman->sourceRect->Width;
	}
	else if (_pacman->position->Y + _pacman->sourceRect->Height > 770)
	{
		_pacman->position->Y = -30 + _pacman->sourceRect->Height;
	}
	else if (_pacman->position->Y + _pacman->sourceRect->Height < 30)
	{
		_pacman->position->Y = 770 - _pacman->sourceRect->Height;
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{
	_munchie->CurrentFrameTime += elapsedTime;
	if (_munchie->CurrentFrameTime > _cMunchieFrameTime)
	{
		_munchie->FrameCount++;

		if (_munchie->FrameCount >= 2)
			_munchie->FrameCount = 0;

		_munchie->CurrentFrameTime = 0;
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
		_pacman->position->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacman->direction = 0;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
		if (_pacman->currentFrameTime > _cPacmanFrameTime)
		{
			_pacman->frame++;
			if (_pacman->frame >= 2)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
			_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
		}
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_pacman->position->X -= _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacman->direction = 2;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
		if (_pacman->currentFrameTime > _cPacmanFrameTime)
		{
			_pacman->frame++;
			if (_pacman->frame >= 2)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
			_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
		}
	}
	// Checks if W key is pressed
	else	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
		_pacman->direction = 3;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
		if (_pacman->currentFrameTime > _cPacmanFrameTime)
		{
			_pacman->frame++;
			if (_pacman->frame >= 2)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
			_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
		}
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
		_pacman->direction = 1;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
		if (_pacman->currentFrameTime > _cPacmanFrameTime)
		{
			_pacman->frame++;
			if (_pacman->frame >= 2)
				_pacman->frame = 0;
			_pacman->currentFrameTime = 0;
			_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
		}
	}
}
void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman

	if (_munchieFrameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchie->InvertedTexture, _munchie->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_munchieFrameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchie->BlueTexture, _munchie->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_munchie->FrameCount++;

		if (_munchie->FrameCount >= 60)
			_munchie->FrameCount = 0;
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