#include "Pacman.h"
#include <sstream>
#include <time.h>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cBananaFrameTime(500), _cappleFrameTime(500)
{
	srand(time(NULL));
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
	}
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{ 
	_munchies[i] = new Enemy();
	_munchies[i]->FrameCount = rand() % 1;
	_munchies[i]->CurrentFrameTime = 0;
	_munchies[i]->frameTime = rand() % 500 + 500;
	}
	ghostDirec = 0;
	count = 0;
	count_tile = 0;
	_pacman = new Player();
	_pacman->dead = false;
	_paused = false;
	_pKeyDown = false;
	_start = true;
	_SKeyDown = false;
	_munchieCurrentFrameTime = 0; 
	_bananacurrentFrameTime = 0;
	_applecurrentFrameTime = 0;
	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->currentFrameTime = 0;
	_pacman->speedMultiplier = 1.0f;
	_pop = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 637, 720, false, 25, 25, "Antman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

	int Tiles[10][5] = {
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
		{1, 1, 1,1,1},
	};
}

Pacman::~Pacman()
{
	delete _bananaTexture;
	delete _bananaSourceRect;
	delete _bananaPosition;
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pop;
	for (int nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->MunchieTex;
		delete _munchies[nCount]->Rect;
	}
	delete[] _munchies;
}

void Pacman::LoadContent()
{
	// Audio
	_pop->Load("Sound/pop.wav");
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.png", false);
	_pacman->position = new Vector2(300.0f, 250.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 48, 48);

	// Load Ghost
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", false);
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()),
			(rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}
	// Load Munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->Munchie = new Texture2D();
		_munchies[i]->MunchieInverted = new Texture2D();
		_munchies[i]->MunchieInverted->Load("Textures/MunchieInverted.tga", false);
		_munchies[i]->Munchie->Load("Textures/Munchie.tga", false);
		_munchies[i]->MunchieTex = new Texture2D();
		_munchies[i]->MunchieTex->Load("Textures/MunchieCombined.tga", false);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->Rect = new Rect(_munchies[i]->position->X,_munchies[i]->position->Y, 12, 12);
	}
	
	// Load Banana
	_bananaTexture = new Texture2D();
	_bananaTexture->Load("Textures/Banana.tga", false);
	_bananaPosition = new Vector2(550.0f, 400.0f);
	_bananaSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Apple
	_appleTexture = new Texture2D();
	_appleTexture->Load("Textures/apple.tga", false);
	_applePosition = new Vector2(50.0f, 400.0f);
	_appleSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	// Set Menu Paramters

	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);


	_Background = new Texture2D();
	_Background->Load("Textures/Background.png", false);
	_Rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
}


void Pacman::Update(int elapsedTime)
{
	Input::MouseState* mouseState = Input::Mouse::GetState();
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	_pacman->currentFrameTime += elapsedTime;
	
	if (!_start)
	{
		if (!_pacman->dead)
		{

			CheckPaused(keyboardState, Input::Keys::P);
			if (!_paused)
			{
				Input(elapsedTime, keyboardState, mouseState);
				UpdatePacman(elapsedTime);
				UpdateBananaAndApple(elapsedTime);
				for (int i = 0; i < GHOSTCOUNT; i++)
					UpdateGhost(_ghosts[i], elapsedTime);

				CheckGhostCollisions();
				for (int i = 0; i < MUNCHIECOUNT; i++)
				{

					if (CheckViewportCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->Rect->Width, _munchies[i]->Rect->Height))
					{
						_munchies[i]->position->Y = -100;
						_munchies[i]->position->X = -100;
						_munchies[i]->Rect = new Rect(_munchies[i]->position->X, _munchies[i]->position->Y, 12, 12);
						count += 1;
						Audio::Play(_pop);
					}
					UpdateMunchie(_munchies[i], elapsedTime);
				}
			}
		}
	}

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && !_SKeyDown)
	{
		_SKeyDown = true;
		_start = !_start;
		_pacman->dead = false;
	}
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	if (ghost->position->Y > _pacman->position->Y)
	{ 
		if (ghost->position->Y + 100 < _pacman->position->Y)
		{
			for (int i = 0; i < GHOSTCOUNT; i++)
				GhostFollow(_ghosts[i], elapsedTime);
		}
	}
	else if (ghost->position->Y > _pacman->position->Y)
	{
		if (ghost->position->Y - 100 < _pacman->position->Y)
		{
			for (int i = 0; i < GHOSTCOUNT; i++)
				GhostFollow(_ghosts[i], elapsedTime);
		}
	}
	else if (ghost->position->X < _pacman->position->X)
	{
		if (ghost->position->X + 100 > _pacman->position->X)
		{
			for (int i = 0; i < GHOSTCOUNT; i++)
				GhostFollow(_ghosts[i], elapsedTime);
		}
	}
		else if (ghost->position->X > _pacman->position->X)
	{ 
			if (ghost->position->X - 100 < _pacman->position->X)
			{
				for (int i = 0; i < GHOSTCOUNT; i++)
					GhostFollow(_ghosts[i], elapsedTime);
			}
	}


	if (ghost->direction == 0) //moves Right
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1) //Moves Left
	{
		ghost->position->X -= ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 2) // moves up
	{
		ghost->position->Y += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 3) // moves down
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
	}
	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits Right edge
	{
		ghost->direction = 1; //Change direction
	}
	else if (ghost->position->X <= 0) // Hits left edge
	{
		ghost->direction = 0; // change direction
	}
	if (ghost->position->Y + ghost->sourceRect->Height > Graphics::GetViewportHeight()) //Hits top 
	{
		ghost->direction = 2; //Change direction
	}
	else if (ghost->position->Y >= 700) // Hits bottom 
	{
		ghost->direction = 2; // change direction
	}
	
}

void Pacman::GhostFollow(MovingEnemy* ghost, int elapsedTime)
{
	if (ghostDirec == 0)
	{ 
	if (ghost->position->Y < _pacman->position->Y)
	ghost->direction = 2;

	else 
		ghost->direction = 3;

	if (ghost->position->Y == _pacman->position->Y)
		ghostDirec = 1;
	}
	
	
	if (ghostDirec == 1)
	{ 
	if (ghost->position->X > _pacman->position->X)
	ghost->direction = 1;

	else 
		ghost->direction = 0;

	if (ghost->position->X == _pacman->position->X)
		ghostDirec = 0;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
	{
		_pacman->position->X = -30 + _pacman->sourceRect->Width;
	}
	else if (_pacman->position->X + _pacman->sourceRect->Width < 25)
	{
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}
	else if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
	{
		_pacman->position->Y = -30 + _pacman->sourceRect->Height;
	}
	else if (_pacman->position->Y + _pacman->sourceRect->Height < 30)
	{
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}
}

void Pacman::UpdateMunchie(Enemy*, int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->CurrentFrameTime += elapsedTime;
		if (_munchies[i]->CurrentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->FrameCount++;

			if (_munchies[i]->FrameCount >= 2)
				_munchies[i]->FrameCount = 0;

			_munchies[i]->CurrentFrameTime = 0;
		}
	}
}
void Pacman::UpdateBananaAndApple(int elapsedTime)
{
	_bananacurrentFrameTime += elapsedTime;
	if (_bananacurrentFrameTime > _cBananaFrameTime)
	{
		_bananaFrameCount++;
		if (_bananaFrameCount == 4)
			_bananaFrameCount = 0;

		_bananacurrentFrameTime = 0;
	}

	_bananacurrentFrameTime += elapsedTime;
	if (_bananacurrentFrameTime > _cBananaFrameTime)
	{
		_bananaFrameCount++;
		if (_bananaFrameCount == 4)
			_bananaFrameCount = 0;

		_bananacurrentFrameTime = 0;
	}
}

bool Pacman::CheckViewportCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	//(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->Rect->Width, _munchies[i]->Rect->Height
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2)
		return false;
	else if (top1 > bottom2)
		return false;
	else if (right1 < left2)
		return false;
	else if (left1 > right2)
		return false;
	else

	return true;
}

void Pacman::CheckGhostCollisions()
{
	//Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		bottom2 =
			_ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 =
			_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
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

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	mouseState->LeftButton;
	mouseState->RightButton;
	enum class S2D_API ButtonState
	{
		RELEASED = 0,
		PRESSED
	};
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_bananaPosition->X = mouseState->X;
		_bananaPosition->Y = mouseState->Y;
	}
	if (mouseState->RightButton == Input::ButtonState::PRESSED)
	{
		_applePosition->X = mouseState->X;
		_applePosition->Y = mouseState->Y;
	}


	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		//Apply multiplier
		_pacman->speedMultiplier = 5.0f;
	}
	else
	{
		//Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}
	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 0;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	}
	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_pacman->position->X -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 2;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	}
	// Checks if W key is pressed
	else	if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_pacman->position->Y -= pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 3;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	}
	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 1;
		_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	}
	// Moves pacman across the X axis
	if (_pacman->direction == 0)
	{ 
		_pacman->position->X += pacmanSpeed;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{

		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	}
	}
	// Moves pacman across the Y axis
	else if (_pacman->direction == 1)
		_pacman->position->Y += pacmanSpeed;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	}
	// Moves pacman across the X axis
	else if (_pacman->direction == 2)
		_pacman->position->X -= pacmanSpeed;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	}
	// Moves pacman across the Y axis
	else if (_pacman->direction == 3)
		_pacman->position->Y -= pacmanSpeed;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
		_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	}

}
void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	std::stringstream Count;
	stream << "ANTman X: " << _pacman->position->X << " Y: " << _pacman->position->Y << "    ";
	Count << "POINTS: " << count;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_Background, _Rectangle, nullptr);
	for (int i = 0; i < GHOSTCOUNT; i++)
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}
	SpriteBatch::Draw(_bananaTexture, _bananaPosition, _bananaSourceRect);
	SpriteBatch::Draw(_appleTexture, _applePosition, _appleSourceRect);
	for (int y = 0; y <= 10; ++y)
	{
		for (int x = 0; x <= 5; ++x)
		{
			//Tile(x, y);
			_tile = new tile;
			_tile->position = new Vector2((32 * count_tile), (32 * count_tile));
			_tile->sourceRect = new Rect(_tile->position->Y, _tile->position->X, 32, 32);
			/*count_tile += 1;
			switch (Tiles[x][y])
			{
			case 1:*/
				_tile->Texture->Load("Textures/Brick_Block.png", true);
			case 0:
				;
			}
			SpriteBatch::Draw(_tile->Texture,_tile->sourceRect);
		}
		if (_bananaFrameCount == 0)
		{
			_bananaSourceRect = new Rect(32.0f, 32.0f, 32, 32);
			_appleSourceRect = new Rect(0.0f, 0.0f, 32, 32);
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchies[i]->Munchie, _munchies[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
		}
		else if (_bananaFrameCount == 1)
		{
			_bananaSourceRect = new Rect(0.0f, 32.0f, 32, 32);
			_appleSourceRect = new Rect(32.0f, 0.0f, 32, 32);
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchies[i]->MunchieInverted, _munchies[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
		}
		else if (_bananaFrameCount == 2)
		{
			_bananaSourceRect = new Rect(32.0f, 0.0f, 32, 32);
			_appleSourceRect = new Rect(0.0f, 32.0f, 32, 32);
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchies[i]->Munchie, _munchies[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
		}
		else if (_bananaFrameCount == 3)
		{
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				SpriteBatch::Draw(_munchies[i]->MunchieInverted, _munchies[i]->Rect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
			_bananaSourceRect = new Rect(0.0f, 0.0f, 32, 32);
			_appleSourceRect = new Rect(32.0f, 32.0f, 32, 32);
		}


		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		SpriteBatch::DrawString(Count.str().c_str(), new Vector2(250.0f, 25.0f), Color::Red);
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
		if (_pacman->dead)
		{
			std::stringstream menuStream; menuStream << "You have Died! (Press space)";
			SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
		}
		SpriteBatch::EndDraw(); // Ends Drawing
	}
}
void Pacman::Tile(int x, int y)
{
			_tile = new tile;
			_tile->position = new Vector2((32 * count_tile), (32 * count_tile));
			_tile->sourceRect = new Rect(_tile->position->Y, _tile->position->X, 32, 32);
			LoadTile(Tiles[y][x], _tile->position->X, _tile->position->Y);
			count_tile += 1;
}
	void Pacman::LoadTile(const char tileType, int x, int y)
	{
		switch (tileType)
		{
		case 1:
			_tile->Texture->Load("Textures/Brick_Block",true);
		case 0:
			;
		}

	}
	//void Pacman::DrawTiles()
	//{
	//	for (int y = 0; y < tile::Height; ++y)
	//	{
	//		for (int x = 0; x < tile::Width; ++x)
	//		{
	//			// If there is a visible tile in that position
	//			Texture2D* texture = _tile->at(x).at(y)->Texture;
	//			if (texture != nullptr)
	//			{
	//				// Draw it in screen space.
	//				Vector2 position(x, y);
	//				position *= *tile::Size;
	//				SpriteBatch::Draw(texture, &position);
	//			}
	//		}
	//	}
	//}


