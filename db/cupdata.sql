-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jan 08, 2016 at 07:33 PM
-- Server version: 5.5.46-0ubuntu0.14.04.2
-- PHP Version: 5.5.9-1ubuntu4.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `cupdata`
--

-- --------------------------------------------------------

--
-- Table structure for table `author`
--

CREATE TABLE IF NOT EXISTS `author` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(100) COLLATE utf8_bin NOT NULL,
  `date` date NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `author_chgs`
--

CREATE TABLE IF NOT EXISTS `author_chgs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `aid` int(11) NOT NULL,
  `cid` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `aid` (`aid`),
  KEY `cid` (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `chgs`
--

CREATE TABLE IF NOT EXISTS `chgs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `chgs` varchar(20) COLLATE utf8_bin NOT NULL,
  `description` varchar(200) COLLATE utf8_bin NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `file`
--

CREATE TABLE IF NOT EXISTS `file` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `filename` varchar(100) COLLATE utf8_bin NOT NULL,
  `type` varchar(10) COLLATE utf8_bin NOT NULL,
  `path` varchar(500) COLLATE utf8_bin NOT NULL,
  `date` date NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `file_author`
--

CREATE TABLE IF NOT EXISTS `file_author` (
  `id` int(11) NOT NULL,
  `fid` bigint(20) NOT NULL,
  `aid` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL,
  KEY `fid` (`fid`),
  KEY `aid` (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

-- --------------------------------------------------------

--
-- Table structure for table `file_chgs`
--

CREATE TABLE IF NOT EXISTS `file_chgs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `fid` bigint(20) NOT NULL,
  `cid` int(11) NOT NULL,
  `timestamp` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fid` (`fid`),
  KEY `cid` (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `version`
--

CREATE TABLE IF NOT EXISTS `version` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `version` varchar(20) COLLATE utf8_bin NOT NULL,
  `fid` bigint(20) NOT NULL,
  `cid` int(11) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `fid` (`fid`),
  KEY `cid` (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=1 ;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `author_chgs`
--
ALTER TABLE `author_chgs`
  ADD CONSTRAINT `author_chgs_ibfk_2` FOREIGN KEY (`cid`) REFERENCES `chgs` (`id`),
  ADD CONSTRAINT `author_chgs_ibfk_1` FOREIGN KEY (`aid`) REFERENCES `author` (`id`);

--
-- Constraints for table `file_author`
--
ALTER TABLE `file_author`
  ADD CONSTRAINT `file_author_ibfk_2` FOREIGN KEY (`aid`) REFERENCES `author` (`id`),
  ADD CONSTRAINT `file_author_ibfk_1` FOREIGN KEY (`fid`) REFERENCES `file` (`id`);

--
-- Constraints for table `file_chgs`
--
ALTER TABLE `file_chgs`
  ADD CONSTRAINT `file_chgs_ibfk_2` FOREIGN KEY (`cid`) REFERENCES `chgs` (`id`),
  ADD CONSTRAINT `file_chgs_ibfk_1` FOREIGN KEY (`fid`) REFERENCES `file` (`id`);

--
-- Constraints for table `version`
--
ALTER TABLE `version`
  ADD CONSTRAINT `version_ibfk_2` FOREIGN KEY (`cid`) REFERENCES `chgs` (`id`),
  ADD CONSTRAINT `version_ibfk_1` FOREIGN KEY (`fid`) REFERENCES `file` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
