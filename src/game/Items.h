#pragma once

#include <fmt/format.h>
#include <sampgdk/core.h>

class IItem {
public:
    int ID = -1;
    std::string Name = "";
    int Model = -1;
    int IDGun = -1;
    int Size = -1;
    int DefaultAmount = -1;
    int MaxAmount = -1;
    float BonusDamage = -1;
};

class Empty : public IItem {
public:
    Empty() {
        ID = 0;
        Name = "Vacio";
        Model = 0;
        IDGun = 0;
        Size = -1;
        DefaultAmount = 0;
        MaxAmount = 0;
        BonusDamage = 0;
    }
};

// Weapons

class Knuckles : public IItem {
public:
    Knuckles() {
        ID = 1;
        Name = "Nudilleras";
        Model = 331;
        IDGun = 1;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Golf : public IItem {
public:
    Golf() {
        ID = 2;
        Name = "Palo de Golf";
        Model = 333;
        IDGun = 2;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Nightstick : public IItem {
public:
    Nightstick() {
        ID = 3;
        Name = "Nightstick";
        Model = 334;
        IDGun = 3;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Knife : public IItem {
public:
    Knife() {
        ID = 4;
        Name = "Navaja";
        Model = 335;
        IDGun = 4;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Bat : public IItem {
public:
    Bat() {
        ID = 5;
        Name = "Bate de madera";
        Model = 336;
        IDGun = 5;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Shovel : public IItem {
public: 
    Shovel() {
        ID = 6;
        Name = "Pala";
        Model = 337;
        IDGun = 6;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Pool : public IItem {
public:
    Pool() {
        ID = 7;
        Name = "Palo de Pool";
        Model = 338;
        IDGun = 7;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Katana : public IItem {
public: 
    Katana() {
        ID = 8;
        Name = "Katana";
        Model = 339;
        IDGun = 8;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Chainsaw : public IItem {
public:
    Chainsaw() {
        ID = 9;
        Name = "Motosierra";
        Model = 341;
        IDGun = 9;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class PurpleDildo : public IItem {
public:
    PurpleDildo() {
        ID = 10;
        Name = "Dildo purpura";
        Model = 321;
        IDGun = 10;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Dildo : public IItem {
public:
    Dildo() {
        ID = 11;
        Name = "Dildo";
        Model = 322;
        IDGun = 11;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Flowers : public IItem {
public:
    Flowers() {
        ID = 12;
        Name = "Flores";
        Model = 325;
        IDGun = 14;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Grenade : public IItem {
public:
    Grenade() {
        ID = 13;
        Name = "Granada";
        Model = 342;
        IDGun = 16;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class GrenadeGas : public IItem {
public:
    GrenadeGas() {
        ID = 14;
        Name = "Granada de Gas";
        Model = 343;
        IDGun = 17;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Molotov : public IItem {
public:
    Molotov() {
        ID = 15;
        Name = "Botella Molotov";
        Model = 344;
        IDGun = 18;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Glock19 : public IItem {
public:
    Glock19() {
        ID = 16;
        Name = "Glock-19 9mm";
        Model = 346;
        IDGun = 22;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 15;
        BonusDamage = 0;
    }
};

class Glock19Silenced : public IItem {
public:
    Glock19Silenced() {
        ID = 17;
        Name = "Glock-19 9mm Silenciada";
        Model = 347;
        IDGun = 23;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 15;
        BonusDamage = 0;
    }
};

class DesertEagle : public IItem {
public:
    DesertEagle() {
        ID = 18;
        Name = "Desert Eagle .50";
        Model = 348;
        IDGun = 24;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 7;
        BonusDamage = 0;
    }
};

class Remington870 : public IItem {
public:
    Remington870() {
        ID = 19;
        Name = "Remington 870 Express";
        Model = 349;
        IDGun = 25;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 8;
        BonusDamage = 0;
    }
};

class Winchester24 : public IItem {
public:
    Winchester24() {
        ID = 20;
        Name = "Winchester Model 24 .12";
        Model = 350;
        IDGun = 26;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 2;
        BonusDamage = 0;
    }
};

class Spas12 : public IItem {
public:
    Spas12() {
        ID = 21;
        Name = "SPAS-12 18,5mm";
        Model = 351;
        IDGun = 27;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 9;
        BonusDamage = 0;
    }
};

class UZI : public IItem {
public:
    UZI() {
        ID = 22;
        Name = "UZI 9mm Parabellum";
        Model = 352;
        IDGun = 28;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 25;
        BonusDamage = 0;
    }
};

class MP5 : public IItem {
public:
    MP5() {
        ID = 23;
        Name = "HK MP5 .22";
        Model = 353;
        IDGun = 29;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 25;
        BonusDamage = 0;
    }
};

class AK47 : public IItem {
public:
    AK47() {
        ID = 24;
        Name = "Kalashnikov Rifle 7,62mm";
        Model = 355;
        IDGun = 30;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 30;
        BonusDamage = 0;
    }
};

class AR15 : public IItem {
public:
    AR15() {
        ID = 25;
        Name = "AR-15 5,56mm";
        Model = 356;
        IDGun = 31;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 30;
        BonusDamage = 0;
    }
};

class TEC9 : public IItem {
public:
    TEC9() {
        ID = 26;
        Name = "TEC-DC9 9mm";
        Model = 372;
        IDGun = 32;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 32;
        BonusDamage = 0;
    }
};

class Rifle : public IItem {
public:
    Rifle() {
        ID = 27;
        Name = "ZKB 680 Fox 16,5mm";
        Model = 357;
        IDGun = 33;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 4;
        BonusDamage = 0;
    }
};

class Sniper : public IItem {
public:
    Sniper() {
        ID = 28;
        Name = "Springfield M1 Garand .308";
        Model = 358;
        IDGun = 34;
        Size = 3;
        DefaultAmount = 1;
        MaxAmount = 5;
        BonusDamage = 0;
    }
};

class Spray : public IItem {
public:
    Spray() {
        ID = 29;
        Name = "Aerosol de pintura";
        Model = 365;
        IDGun = 41;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 999;
        BonusDamage = 0;
    }
};

class Extintor : public IItem {
public:
    Extintor() {
        ID = 30;
        Name = "Matafuego";
        Model = 366;
        IDGun = 42;
        Size = 2;
        DefaultAmount = 1;
        MaxAmount = 999;
        BonusDamage = 0;
    }
};

class Camera : public IItem {
public:
    Camera() {
        ID = 31;
        Name = "Camera T50";
        Model = 367;
        IDGun = 43;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class Taser : public IItem {
public:
    Taser() {
        ID = 32;
        Name = "Taser";
        Model = 347;
        IDGun = 23;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

// Weapons Variations
class SigP226 : public IItem {
public:
    SigP226() {
        ID = 33;
        Name = "Sig Sauer P226 9mm";
        Model = 346;
        IDGun = 22;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 17;
        BonusDamage = 0;
    }
};

class SigP250 : public IItem {
public:
    SigP250() {
        ID = 34;
        Name = "Sig Sauer P250 Compact 9mm";
        Model = 346;
        IDGun = 22;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 15;
        BonusDamage = 0;
    }
};

class Glock17 : public IItem {
public:
    Glock17() {
        ID = 35;
        Name = "Glock-17 9mm";
        Model = 346;
        IDGun = 22;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 17;
        BonusDamage = 0;
    }
};

class Glock22 : public IItem {
public:
    Glock22() {
        ID = 36;
        Name = "Glock-22 .45 ACP";
        Model = 348;
        IDGun = 24;
        Size = 1;
        DefaultAmount = 1;
        MaxAmount = 15;
        BonusDamage = 0;
    }
};

// Items
class SamsungOrange : public IItem {
public:
    SamsungOrange() {
        ID = 50;
        Name = "Samsung (Naranja)";
        Model = 18865;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungSkyblue : public IItem {
public:
    SamsungSkyblue() {
        ID = 51;
        Name = "Samsung (Celeste)";
        Model = 18866;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungBlack : public IItem {
public:
    SamsungBlack() {
        ID = 52;
        Name = "Samsung (Negro)";
        Model = 18868;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungPink : public IItem {
public:
    SamsungPink() {
        ID = 53;
        Name = "Samsung (Rosa)";
        Model = 18869;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungRed : public IItem {
public:
    SamsungRed() {
        ID = 54;
        Name = "Samsung (Rojo)";
        Model = 18870;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungGreen : public IItem {
public:
    SamsungGreen() {
        ID = 55;
        Name = "Samsung (Verde)";
        Model = 18871;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungBlue : public IItem {
public:
    SamsungBlue() {
        ID = 56;
        Name = "Samsung (Azul)";
        Model = 18872;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungYellow : public IItem {
public:
    SamsungYellow() {
        ID = 57;
        Name = "Samsung (Amarillo)";
        Model = 18873;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class SamsungWhite : public IItem {
public:
    SamsungWhite() {
        ID = 58;
        Name = "Samsung (Blanco)";
        Model = 18874;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class IphoneBlack : public IItem {
public:
    IphoneBlack() {
        ID = 59;
        Name = "Iphone (Negro)";
        Model = 18868;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class IphoneBlue : public IItem {
public:
    IphoneBlue() {
        ID = 60;
        Name = "Iphone (Blanco)";
        Model = 18872;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

class IphoneWhite : public IItem {
public:
    IphoneWhite() {
        ID = 61;
        Name = "Iphone (Blanco)";
        Model = 18874;
        IDGun = 0;
        DefaultAmount = 1;
        MaxAmount = 1;
        BonusDamage = 0;
    }
};

static IItem* GetItemByGun(int id) {
    switch(id) {
        case 0: return new Empty();
        case 1: return new Knuckles();
        case 2: return new Golf();
        case 3: return new Nightstick();
        case 4: return new Knife();
        case 5: return new Bat();
        case 6: return new Shovel();
        case 7: return new Pool();
        case 8: return new Katana();
        case 9: return new Chainsaw();
        case 10: return new PurpleDildo();
        case 11: return new Dildo();
        case 14: return new Flowers();
        case 16: return new Grenade();
        case 17: return new GrenadeGas();
        case 18: return new Molotov();
        case 22: return new Glock19();
        case 23: return new Glock19Silenced();
        case 24: return new DesertEagle();
        case 25: return new Remington870();
        case 26: return new Winchester24();
        case 27: return new Spas12();
        case 28: return new UZI();
        case 29: return new MP5();
        case 30: return new AK47();
        case 31: return new AR15();
        case 32: return new TEC9();
        case 33: return new Rifle();
        case 34: return new Sniper();
        case 41: return new Spray();
        case 42: return new Extintor();
        case 43: return new Camera();

        case 44: return new Taser();
        case 45: return new SigP226();
        case 46: return new SigP250();
        case 47: return new Glock17();
        case 48: return new Glock22();

        default: return new Empty();
    }
};

static IItem* GetItem(int id) {
    switch (id) {
        //Weapons
        case 0: return new Empty();
        case 1: return new Knuckles();
        case 2: return new Golf();
        case 3: return new Nightstick();
        case 4: return new Knife();
        case 5: return new Bat();
        case 6: return new Shovel();
        case 7: return new Pool();
        case 8: return new Katana();
        case 9: return new Chainsaw();
        case 10: return new PurpleDildo();
        case 11: return new Dildo();
        case 12: return new Flowers();
        case 13: return new Grenade();
        case 14: return new GrenadeGas();
        case 15: return new Molotov();
        case 16: return new Glock19();
        case 17: return new Glock19Silenced();
        case 18: return new DesertEagle();
        case 19: return new Remington870();
        case 20: return new Winchester24();
        case 21: return new Spas12();
        case 22: return new UZI();
        case 23: return new MP5();
        case 24: return new AK47();
        case 25: return new AR15();
        case 26: return new TEC9();
        case 27: return new Rifle();
        case 28: return new Sniper();
        case 29: return new Spray();
        case 30: return new Extintor();
        case 31: return new Camera();
        case 32: return new Taser();
        case 33: return new SigP226();
        case 34: return new SigP250();
        case 35: return new Glock17();
        case 36: return new Glock22();

        //Samsungs
        case 50: return new SamsungOrange();
        case 51: return new SamsungSkyblue();
        case 52: return new SamsungBlack();
        case 53: return new SamsungPink();
        case 54: return new SamsungRed();
        case 55: return new SamsungGreen();
        case 56: return new SamsungBlue();
        case 57: return new SamsungYellow();
        case 58: return new SamsungWhite();

        //Iphones
        case 59: return new IphoneBlack();
        case 60: return new IphoneBlue();
        case 61: return new IphoneWhite();

        //Items
        default: return new Empty();
    }
};

/*
18865 - Telefono naranja
18866 - Telefono celeste
18867 - Telefono naranja rojizo
18868 - Telefono negro
18869 - Telefono rosa
18870 - Telefono rojo
18871 - Telefono verde
18872 - Telefono azul
18873 - Telefono amarillo
18874 - Telefono blanco
*/