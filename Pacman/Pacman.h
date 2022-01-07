#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif
#define MUNCHIECOUNT 50
#define GHOSTCOUNT 3
// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
	//Structure definition
enum class TileCollision
{
	passable = 0,
    impassable = 1,
    platform = 2
};
struct tile
{
	Texture2D* Texture;
	TileCollision Collision;
    static const int Width = 32;
    static const int Height = 32;
	const Vector2* tile::Size = new Vector2(Width,Height);
};
struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct Player
{
	bool dead;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};
struct Enemy
{
	int frameTime;
	int Frame;
	int CurrentFrameTime;
	int FrameCount;
	Rect* Rect;
	Texture2D* MunchieInverted;
	Texture2D* Munchie;
	Texture2D* MunchieTex;
	Vector2* position;
};
class Pacman : public Game
{

private:

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Enemy*, int elapsedTime);
	void UpdateBananaAndApple(int elapsedTime);
	void DeadPacman(int elapsedTime);
	// Data for Menu

	void CheckGhostCollisions();
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	void GhostFollow(MovingEnemy*, int elapsedTime);

	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Texture2D* _Background;
	Rect* _Rectangle;
	Vector2* _menuStringPosition;
	Player* _pacman;
	tile* _tile;
	Enemy* _munchies[MUNCHIECOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];
	bool _paused;
	bool _pKeyDown;
	int _pacmanDirection;
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;
	int _munchieFrame;
	int _munchieCurrentFrameTime;
	int count;
	int ghostDirec;

	SoundEffect* _pop;
	int _bananaFrameCount;
	const int _cBananaFrameTime;
	int _bananaframe;
	int _bananacurrentFrameTime;

	int appleFrameCount;
	const int  _cappleFrameTime;
	int  _appleframe;
	int _applecurrentFrameTime;

	bool _start;
	bool _SKeyDown;
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;

	// Data to represent Banana & apple
	Vector2* _bananaPosition;
	Rect* _bananaSourceRect;
	Texture2D* _bananaTexture;

	Vector2* _applePosition;
	Rect* _appleSourceRect;
	Texture2D* _appleTexture;

	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables 
	const float _cPacmanSpeed;

public:

	void LoadTiles(int levelIndex);
	void LoadTile(const char tileType, int x, int y);
	void DrawTiles();

	void Tile(Texture2D* texture, TileCollision collision);

	bool CheckViewportCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};