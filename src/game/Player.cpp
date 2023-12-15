#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include <fmt/format.h>
#include "Player.h"
#include "../utils/Point.h"
#include "../utils/Constants.h"
#include "../utils/Util.h"

using namespace std;

Player::Player(int id) : id(id)
{
	SetPlayerSkin(id, skin);
}


Player::~Player()
{

}

string Player::ParseName() {
	std::vector<std::string> _name = splice(name, '_');

	return fmt::format("{} {}", _name[0], _name[1]);
}

// Account
void Player::SetAccountID(int _id) {
	aID = _id;
}

int Player::GetAccountID() {
	return aID;
}

void Player::SetNick(std::string _nick) {
	nick = _nick;
}

std::string Player::GetNick() {
	return nick;
}

void Player::SetEmail(std::string _email) {
	email = _email;
}

std::string Player::GetEmail() {
	return email;
}

bool Player::HasValidEmail(std::string _email) {
	std::size_t found = _email.find("@") || _email.find(".");
	return (found != std::string::npos);
}

void Player::SetPassword(std::string _password) {
	password = _password;
}

std::string Player::GetPassword() {
	return password;
}

void Player::SetSalt(std::string _salt) {
	salt = _salt;
}

std::string Player::GetSalt() {
	return salt;
}

void Player::SetIP(std::string _ip) {
	ip = _ip;
}

std::string Player::GetIP() {
	return ip;
}

// Functions

void Player::StartWalkAnimation(int index)
{
	static const char* WALK_ANIMATIONS[] =
	{
		"WALK_GANG1",
		"WALK_GANG2",
		"WALK_FAT",
		"WALK_FATOLD",
		"WALK_OLD",
		"WOMAN_WALKNORM",
		"WOMAN_WALKSEXY",
		"WALK_WUZI"
	};
	const int MAX_WALKS = ARRAY_SIZE(WALK_ANIMATIONS);

	if (index < MAX_WALKS)
	{
		StartAnimation("PED", WALK_ANIMATIONS[index], 4.1f, true, true, true, true, 1, true);
	}
}

void Player::StartRunAnimation(int index)
{
	static const char* RUN_ANIMATIONS[] =
	{
		"FATRUN",
		"MUSCLERUN",
		"RUN_CIVI",
		"RUN_OLD",
		"RUN_GANG1",
		"WOMAN_RUN",
		"WOMAN_RUNPANIC"
	};
	const int MAX_RUNS = ARRAY_SIZE(RUN_ANIMATIONS);

	if (index < MAX_RUNS)
	{
		StartAnimation("PED", RUN_ANIMATIONS[index], 4.1f, true, true, true, true, 1, true);
	}
}

void Player::StartAnimation(const char* animationLibrary, const char* animationName, float delta, bool loop, bool lockX, bool lockY, bool freeze, int time, bool forceSync)
{
	isLoopingAnimation = loop;
	ApplyAnimation(id, animationLibrary, animationName, delta, loop, lockX, lockY, freeze, time, forceSync);

	if (isLoopingAnimation)
	{
		GameTextForPlayer(id, "~p~Press the sprint key to stop the animation.", 3000, 1);
	}
}

void Player::StopAnimations()
{
	//ApplyAnimation(id, "CARRY", "crry_prtial", 4.f, false, false, false, false, false, true); // clears animations
	ClearAnimations(id, true);
}

bool Player::IsLoopingAnimation()
{
	return isLoopingAnimation;
}

void Player::Kill()
{
	SetPlayerHealth(id, 0);
}

void Player::GiveWeapon(int weaponID)
{
	GiveWeapon(weaponID, 1500);
}
void Player::GiveWeapon(int weaponID, int ammo)
{
	GivePlayerWeapon(id, weaponID, ammo);
}

bool Player::IsInRangeOfPlayer(float range, Player* player)
{
	return IsInRangeOfPoint(range, player->GetPosition());
}

bool Player::IsInRangeOfPoint(float range, Point3D point)
{
	return IsPlayerInRangeOfPoint(id, range, point.x, point.y, point.z);
}

float Player::GetDistanceFromPoint(Point3D point)
{
	return GetPlayerDistanceFromPoint(id, point.x, point.y, point.z);
}

bool Player::IsInVehicle()
{
	return IsPlayerInAnyVehicle(id);
}

int Player::GetVehicleID()
{
	return GetPlayerVehicleID(id);
}

int Player::GetVehicleModelID()
{
	return GetVehicleModel(GetVehicleID());
}

bool Player::IsInVehicleID(int vehicleID)
{
	if (IsInVehicle()) {
		return (GetVehicleID() == vehicleID);
	}
	return false;
}