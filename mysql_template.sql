-- Basic MySQL template for the CSG2 frontend of MediaCourier

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

CREATE DATABASE IF NOT EXISTS `mediacourier` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin;
USE `mediacourier`;

CREATE TABLE IF NOT EXISTS `oosers` (
  `username` varchar(64) COLLATE utf8_bin NOT NULL COMMENT 'Username',
  `passwdhash` varchar(128) COLLATE utf8_bin NOT NULL COMMENT,
  `salt` int(10) unsigned NOT NULL COMMENT 'Randomly generated salt',
  `streamkey` varchar(40) COLLATE utf8_bin NOT NULL,
  `lastonline` bigint(20) NOT NULL COMMENT '64-bit unix timestamp, GMT',
  `registered` bigint(20) NOT NULL COMMENT '64-bit unix timestamp, GMT',
  `userlevel` enum('ADMIN','RESTRICTED','USER','VIP') COLLATE utf8_bin NOT NULL COMMENT 'User privilege level'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

ALTER TABLE `oosers`
  ADD PRIMARY KEY (`username`),
  ADD UNIQUE KEY `streamkey` (`streamkey`),
  ADD KEY `userlevel` (`userlevel`);

COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
