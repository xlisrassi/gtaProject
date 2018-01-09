-- phpMyAdmin SQL Dump
-- version 2.10.3
-- http://www.phpmyadmin.net
-- 
-- โฮสต์: localhost
-- เวลาในการสร้าง: 
-- รุ่นของเซิร์ฟเวอร์: 5.0.51
-- รุ่นของ PHP: 5.2.6

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

-- 
-- ฐานข้อมูล: `174_bbasni_5050`
-- 

-- --------------------------------------------------------

-- 
-- โครงสร้างตาราง `bans`
-- 

CREATE TABLE `bans` (
  `id` int(11) NOT NULL auto_increment,
  `type` tinyint(2) NOT NULL,
  `player` int(11) NOT NULL,
  `time` int(11) NOT NULL,
  `amount` bigint(20) NOT NULL default '0',
  `ip` varchar(16) NOT NULL,
  `inactive` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- 
-- dump ตาราง `bans`
-- 


-- --------------------------------------------------------

-- 
-- โครงสร้างตาราง `logins`
-- 

CREATE TABLE `logins` (
  `id` int(11) NOT NULL auto_increment,
  `time` int(11) NOT NULL,
  `ip` varchar(16) NOT NULL,
  `userid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=96 ;

-- 
-- dump ตาราง `logins`
-- 


-- --------------------------------------------------------

-- 
-- โครงสร้างตาราง `players`
-- 

CREATE TABLE `players` (
  `id` int(11) NOT NULL auto_increment,
  `Name` varchar(50) collate latin1_general_ci NOT NULL,
  `Password` varchar(50) character set latin1 collate latin1_bin NOT NULL,
  `Leve` int(11) NOT NULL default '1',
  `AdminLevel` int(11) NOT NULL default '0',
  `DonateRank` int(11) NOT NULL default '0',
  `UpgradePoints` int(11) NOT NULL default '0',
  `ConnectedTime` int(11) NOT NULL default '0',
  `Carkey` int(11) NOT NULL default '0',
  `Registered` int(11) NOT NULL default '0',
  `Sex` int(11) NOT NULL default '1',
  `Age` int(11) NOT NULL default '0',
  `Origin` int(11) NOT NULL default '255',
  `CK` int(11) NOT NULL default '0',
  `Muted` int(11) NOT NULL default '0',
  `Respect` int(11) NOT NULL default '0',
  `Money` bigint(20) NOT NULL default '500',
  `Bank` int(11) NOT NULL default '1000',
  `Crimes` int(11) NOT NULL default '0',
  `Kills` int(11) NOT NULL default '0',
  `Deaths` int(11) NOT NULL default '0',
  `Arrested` int(11) NOT NULL default '0',
  `WantedDeaths` int(11) NOT NULL default '0',
  `Phonebook` int(11) NOT NULL default '0',
  `LottoNr` int(11) NOT NULL default '0',
  `Fishes` int(11) NOT NULL default '0',
  `BiggestFish` int(11) NOT NULL default '0',
  `Job` int(11) NOT NULL default '0',
  `Paycheck` int(11) NOT NULL default '0',
  `HeadValue` int(11) NOT NULL default '0',
  `Jailed` int(11) NOT NULL default '0',
  `JailTime` int(11) NOT NULL default '0',
  `Materials` int(11) NOT NULL default '0',
  `Drugs` int(11) NOT NULL default '0',
  `Leader` int(11) NOT NULL default '0',
  `Member` int(11) NOT NULL default '0',
  `FMember` int(11) NOT NULL default '255',
  `Rank` int(11) NOT NULL default '0',
  `Chara` int(11) NOT NULL default '0',
  `ContractTime` int(11) NOT NULL default '0',
  `DetSkill` int(11) NOT NULL default '0',
  `SexSkill` int(11) NOT NULL default '0',
  `BoxSkill` int(11) NOT NULL default '0',
  `LawSkill` int(11) NOT NULL default '0',
  `MechSkill` int(11) NOT NULL default '0',
  `JackSkill` int(11) NOT NULL default '0',
  `CarSkill` int(11) NOT NULL default '0',
  `NewsSkill` int(11) NOT NULL default '0',
  `DrugsSkill` int(11) NOT NULL default '0',
  `CookSkill` int(11) NOT NULL default '0',
  `FishSkill` int(11) NOT NULL default '0',
  `pSHealth` varchar(16) collate latin1_general_ci NOT NULL default '50.0',
  `pHealth` varchar(16) collate latin1_general_ci NOT NULL default '50.0',
  `Inte` int(11) NOT NULL,
  `Local` int(11) NOT NULL default '255',
  `Team` int(11) NOT NULL default '3',
  `Model` int(11) NOT NULL default '264',
  `PhoneNr` int(11) NOT NULL default '0',
  `House` int(11) NOT NULL default '255',
  `Bizz` int(11) NOT NULL default '255',
  `Pos_x` varchar(16) collate latin1_general_ci NOT NULL default '1684.9',
  `Pos_y` varchar(16) collate latin1_general_ci NOT NULL default '-2244.5',
  `Pos_z` varchar(16) collate latin1_general_ci NOT NULL default '13.5',
  `CarLic` int(11) NOT NULL default '0',
  `FlyLic` int(11) NOT NULL default '0',
  `BoatLic` int(11) NOT NULL default '0',
  `FishLic` int(11) NOT NULL default '0',
  `GunLic` int(11) NOT NULL default '0',
  `Gun1` int(11) NOT NULL default '0',
  `Gun2` int(11) NOT NULL default '0',
  `Gun3` int(11) NOT NULL default '0',
  `Gun4` int(11) NOT NULL default '0',
  `Ammo1` int(11) NOT NULL default '0',
  `Ammo2` int(11) NOT NULL default '0',
  `Ammo3` int(11) NOT NULL default '0',
  `Ammo4` int(11) NOT NULL default '0',
  `CarTime` int(11) NOT NULL default '0',
  `PayDay` int(11) NOT NULL default '0',
  `PayDayHad` int(11) NOT NULL default '0',
  `CDPlayer` int(11) NOT NULL default '0',
  `Wins` int(11) NOT NULL default '0',
  `Loses` int(11) NOT NULL default '0',
  `AlcoholPerk` int(11) NOT NULL default '0',
  `DrugPerk` int(11) NOT NULL default '0',
  `MiserPerk` int(11) NOT NULL default '0',
  `PainPerk` int(11) NOT NULL default '0',
  `TraderPerk` int(11) NOT NULL default '0',
  `Tutorial` int(11) NOT NULL default '0',
  `Mission` int(11) NOT NULL default '0',
  `Warnings` int(11) NOT NULL default '0',
  `Adjustable` int(11) NOT NULL default '0',
  `Fuel` int(11) NOT NULL default '0',
  `Married` int(11) NOT NULL default '0',
  `MarriedTo` varchar(50) collate latin1_general_ci NOT NULL default 'No-one',
  `iyaHealme` int(11) NOT NULL default '0',
  `PShop` int(11) NOT NULL default '0',
  `Classlevel` int(11) NOT NULL default '0',
  `Good` int(11) NOT NULL default '0',
  `Capsule` int(11) NOT NULL default '0',
  `Kira` int(11) NOT NULL default '0',
  `WinGle` int(11) NOT NULL default '0',
  `LoseGle` int(11) NOT NULL default '0',
  `WS` int(11) NOT NULL default '0',
  `Palak` int(11) NOT NULL default '0',
  `Palak1` int(11) NOT NULL default '0',
  `Palak2` int(11) NOT NULL default '0',
  `Palak3` int(11) NOT NULL default '0',
  `Palak4` int(11) NOT NULL default '0',
  `Palak5` int(11) NOT NULL default '0',
  `Jetpack` int(11) NOT NULL default '0',
  `SonTeen` int(11) NOT NULL default '0',
  `Box` int(11) NOT NULL default '0',
  `SetDragonFlag` int(11) NOT NULL default '0',
  `WaterGun` int(11) NOT NULL default '0',
  `WingColor` int(11) NOT NULL default '0',
  `SetWingGod` int(11) NOT NULL default '0',
  `SetWingHell` int(11) NOT NULL default '0',
  `SetDJ` int(11) NOT NULL default '0',
  `SetWarrior` int(11) NOT NULL default '0',
  `SetPilot` int(11) NOT NULL default '0',
  `SetSoldier` int(11) NOT NULL default '0',
  `WingFire` int(11) NOT NULL default '0',
  `DN` int(11) NOT NULL default '0',
  `ColorVip` int(11) NOT NULL default '0',
  `VirtualWorld` int(11) NOT NULL default '0',
  `Banfac` int(11) NOT NULL default '0',
  `Virus` int(11) NOT NULL default '0',
  `Megaphone` int(11) NOT NULL default '0',
  `NewbieMoney` int(11) NOT NULL default '0',
  `DeagleOn` int(11) NOT NULL default '0',
  `DeagleAmmo` int(11) NOT NULL default '0',
  `Ball` int(11) NOT NULL default '0',
  `Balls` int(11) NOT NULL default '0',
  `BallWin` int(11) NOT NULL default '0',
  `windrift` int(11) NOT NULL default '0',
  `ScoreShop` int(11) NOT NULL default '0',
  `Tribal1` int(11) NOT NULL default '0',
  `Tribal2` int(11) NOT NULL default '0',
  `Tribal3` int(11) NOT NULL default '0',
  `Tribal4` int(11) NOT NULL default '0',
  `Tribal5` int(11) NOT NULL default '0',
  `CPG` int(11) NOT NULL default '0',
  `Eagle` int(11) NOT NULL default '0',
  `AK47` int(11) NOT NULL default '0',
  `M4` int(11) NOT NULL default '0',
  `MP5` int(11) NOT NULL default '0',
  `Sniper` int(11) NOT NULL default '0',
  `Shotgun` int(11) NOT NULL default '0',
  `Combat` int(11) NOT NULL default '0',
  `Sawnoff` int(11) NOT NULL default '0',
  `Tec` int(11) NOT NULL default '0',
  `Silenced` int(11) NOT NULL default '0',
  `9mm` int(11) NOT NULL default '0',
  `SumVIP` int(11) NOT NULL default '0',
  `Soul` int(11) NOT NULL default '0',
  `DBPoint` int(11) NOT NULL default '0',
  `Meed` int(11) NOT NULL default '0',
  `Juti` int(11) NOT NULL default '0',
  `SG` int(11) NOT NULL default '0',
  `CG1` int(11) NOT NULL default '0',
  `CG2` int(11) NOT NULL default '0',
  `CG3` int(11) NOT NULL default '0',
  `CG4` int(11) NOT NULL default '0',
  `BExp` int(11) NOT NULL default '0',
  `DriftPoint` int(11) NOT NULL default '0',
  `LastWanted` int(11) NOT NULL default '0',
  `QuitPao` int(11) NOT NULL default '0',
  `ClearStats` int(11) NOT NULL default '0',
  `Spawn` int(11) NOT NULL default '0',
  `Banned` int(11) NOT NULL,
  `AcOi` int(11) NOT NULL default '0',
  `Skill4` int(11) NOT NULL default '0',
  `Skill9` int(11) NOT NULL default '0',
  `Skill10` int(11) NOT NULL default '0',
  `Skill11` int(11) NOT NULL default '0',
  `Skill12` int(11) NOT NULL default '0',
  `Skill13` int(11) NOT NULL default '0',
  `Skill14` int(11) NOT NULL default '0',
  `Skill17` int(11) NOT NULL default '0',
  `ESCtexto` varchar(50) collate latin1_general_ci NOT NULL default 'No-one',
  `ESCtext` int(11) NOT NULL default '0',
  `Lighter` int(11) NOT NULL default '0',
  `Cigarettes` int(11) NOT NULL default '0',
  `Weeds` int(11) NOT NULL default '0',
  `WeedsSkill` int(11) NOT NULL default '0',
  `BongWeed` int(11) NOT NULL default '0',
  `JailMinutes` int(11) NOT NULL default '0',
  `JailSeconds` int(11) NOT NULL default '0',
  `Hm` int(11) NOT NULL default '0',
  `Idcard` int(11) NOT NULL default '0',
  `Idcardnumber` varchar(16) collate latin1_general_ci NOT NULL default 'No-one',
  `Idcardvalid` int(11) NOT NULL default '0',
  `Likes` int(11) NOT NULL default '0',
  `Institution` int(11) NOT NULL default '0',
  `SantaH` int(11) NOT NULL default '0',
  `GiftHny` int(11) NOT NULL default '0',
  `DbSword` int(11) NOT NULL default '0',
  `Barrett` int(11) NOT NULL default '0',
  `GsXray` int(11) NOT NULL default '0',
  `Dragonfly` int(11) NOT NULL default '0',
  `Windab` int(11) NOT NULL default '0',
  `Losedab` int(11) NOT NULL default '0',
  `NrG500` int(11) NOT NULL default '0',
  `Infernus` int(11) NOT NULL default '0',
  `BalayRed` int(11) NOT NULL default '0',
  `Gle70` int(11) NOT NULL default '0',
  `SetCar` int(11) NOT NULL default '0',
  `Announcer` int(11) NOT NULL default '0',
  `Announcer2` int(11) NOT NULL default '0',
  `TheKing` int(11) NOT NULL default '0',
  `BFKill` int(11) NOT NULL default '0',
  `BFDeath` int(11) NOT NULL default '0',
  `BFRank` int(11) NOT NULL default '0',
  `ProtectB` int(11) NOT NULL default '0',
  `ProtectG` int(11) NOT NULL default '0',
  `M420` int(11) NOT NULL default '0',
  `Kidlv` int(11) NOT NULL default '0',
  `Kidls` int(11) NOT NULL default '0',
  `Boom` int(11) NOT NULL default '0',
  `Sec` int(11) NOT NULL default '0',
  `Guncheck` int(11) NOT NULL default '0',
  `Spay` int(11) NOT NULL default '0',
  `Secspay` int(11) NOT NULL default '0',
  `pfcr900` int(11) NOT NULL default '0',
  `pturismo` int(11) NOT NULL default '0',
  `ppcj600` int(11) NOT NULL default '0',
  `pSultan` int(11) NOT NULL default '0',
  `pHelicopter` int(11) NOT NULL default '0',
  `pBoat` int(11) NOT NULL default '0',
  `Kongfai` int(11) NOT NULL default '0',
  `Boombox` int(11) NOT NULL default '0',
  `Spade1` int(11) NOT NULL default '0',
  `Spade2` int(11) NOT NULL default '0',
  `rTubtim` int(11) NOT NULL default '0',
  `rMorakod` int(11) NOT NULL default '0',
  `rCrystal` int(11) NOT NULL default '0',
  `rThongkum` int(11) NOT NULL default '0',
  `rMari` int(11) NOT NULL default '0',
  `Mp5Dmg` int(11) NOT NULL default '0',
  `Mp5DmgBlock` int(11) NOT NULL default '0',
  `Shotgundmg` int(11) NOT NULL default '0',
  `ShotgundmgBlock` int(11) NOT NULL default '0',
  `pBomb` int(11) NOT NULL default '0',
  `UseSni` int(11) NOT NULL default '0',
  `UseTec` int(11) NOT NULL default '0',
  `MinSni` int(11) NOT NULL default '0',
  `MinTec` int(11) NOT NULL default '0',
  `Bikes` int(11) NOT NULL default '0',
  `BarrettLimited` int(11) NOT NULL,
  `BarrettLimitedBlock` int(11) NOT NULL,
  `NeonError` int(11) NOT NULL,
  `ChangeNameError` int(11) NOT NULL,
  `Pointevent2014` int(11) NOT NULL,
  `Rod1` int(11) NOT NULL,
  `Rod2` int(11) NOT NULL,
  `GiftLevel` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci AUTO_INCREMENT=12 ;

-- 
-- dump ตาราง `players`
-- 


-- --------------------------------------------------------

-- 
-- โครงสร้างตาราง `truemoney`
-- 

CREATE TABLE `truemoney` (
  `card_id` int(6) unsigned NOT NULL auto_increment,
  `password` varchar(14) NOT NULL,
  `user_id` int(11) unsigned NOT NULL,
  `amount` int(4) unsigned NOT NULL,
  `status` tinyint(1) unsigned NOT NULL,
  `added_time` datetime NOT NULL,
  PRIMARY KEY  (`card_id`),
  KEY `user_id` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- 
-- dump ตาราง `truemoney`
-- 

