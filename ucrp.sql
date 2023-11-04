CREATE TABLE Accounts (
	ID INTEGER PRIMARY KEY AUTOINCREMENT,
	Name varchar(24) NOT NULL DEFAULT 'none',
	Password varchar(65) NOT NULL DEFAULT 'none',
	Salt varchar(11) NOT NULL DEFAULT 'none',
	Email varchar(256) NOT NULL DEFAULT '-',
	IP varchar(18) NOT NULL DEFAULT '127.0.0.1'
);

CREATE TABLE Characters (
	ID INTEGER PRIMARY KEY AUTOINCREMENT,
	Nick varchar(24) NOT NULL DEFAULT 'none',
	Account int(11) NOT NULL DEFAULT '-1',
	PositionX float NOT NULL DEFAULT '0',
	PositionY float NOT NULL DEFAULT '0',
	PositionZ float NOT NULL DEFAULT '0',
	PositionR float NOT NULL DEFAULT '0',
	Interior int(11) NOT NULL DEFAULT '0',
	VirtualWorld int(11) NOT NULL DEFAULT '0',
	Health float NOT NULL DEFAULT '100.0',
	Kevlar float NOT NULL DEFAULT '0',
	Skin int(11) NOT NULL DEFAULT '289'
);

CREATE TABlE Inventories (
	ID INTEGER PRIMARY KEY AUTOINCREMENT,
	IDCharacter int (11) NOT NULL,
	Slot1 int(11) NOT NULL DEFAULT '0',
	SlotAmount1 int(11) NOT NULL DEFAULT '0',
	Slot2 int(11) NOT NULL DEFAULT '0',
	SlotAmount2 int(11) NOT NULL DEFAULT '0',
	Slot3 int(11) NOT NULL DEFAULT '0',
	SlotAmount3 int(11) NOT NULL DEFAULT '0',
	Slot4 int(11) NOT NULL DEFAULT '0',
	SlotAmount4 int(11) NOT NULL DEFAULT '0',
	Slot5 int(11) NOT NULL DEFAULT '0',
	SlotAmount5 int(11) NOT NULL DEFAULT '0'
);