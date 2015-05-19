/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50151
Source Host           : localhost:3306
Source Database       : food

Target Server Type    : MYSQL
Target Server Version : 50151
File Encoding         : 65001

Date: 2013-08-27 10:04:06
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `companyinfo`
-- ----------------------------
DROP TABLE IF EXISTS `companyinfo`;
CREATE TABLE `companyinfo` (
  `Company_ID` int(11) NOT NULL AUTO_INCREMENT,
  `Company_Name` char(100) CHARACTER SET utf8 DEFAULT NULL,
  `Company_Tel` char(30) CHARACTER SET utf8 DEFAULT NULL,
  `Company_Email` char(50) DEFAULT NULL,
  `Company_Addr` char(100) CHARACTER SET utf8 DEFAULT NULL,
  `Company_Class` char(100) CHARACTER SET utf8 DEFAULT NULL,
  `Company_Des` char(255) DEFAULT NULL,
  PRIMARY KEY (`Company_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of companyinfo
-- ----------------------------
INSERT INTO `companyinfo` VALUES ('7', 'sdf', '', '', '海淀', '', null);
INSERT INTO `companyinfo` VALUES ('8', '222', '', '', '', '', null);

-- ----------------------------
-- Table structure for `detectioninfo`
-- ----------------------------
DROP TABLE IF EXISTS `detectioninfo`;
CREATE TABLE `detectioninfo` (
  `Info_Id` int(11) NOT NULL AUTO_INCREMENT,
  `batch_id` char(20) DEFAULT NULL,
  `task_id` char(20) DEFAULT NULL,
  `video_path` char(255) DEFAULT NULL,
  `p_name` char(100) DEFAULT NULL,
  `type` char(50) DEFAULT NULL,
  `channel` int(11) DEFAULT NULL,
  `result_unit` char(50) DEFAULT NULL,
  `sample_no` char(50) DEFAULT NULL,
  `sample_name` char(100) DEFAULT NULL,
  `sites` char(100) DEFAULT NULL,
  `submission_unit` char(100) DEFAULT NULL COMMENT '送检单位',
  `test_operator` char(50) DEFAULT NULL,
  `test_time` char(50) DEFAULT NULL,
  `test_unit` char(100) DEFAULT NULL,
  `Juge_result` char(100) DEFAULT NULL,
  `Detect_result` char(100) DEFAULT NULL,
  `standard` char(50) DEFAULT NULL,
  `Absolut_result` char(50) DEFAULT NULL,
  `Device_ID` int(11) DEFAULT NULL,
  `Is_Quli` int(11) DEFAULT NULL,
  PRIMARY KEY (`Info_Id`)
) ENGINE=InnoDB AUTO_INCREMENT=66 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of detectioninfo
-- ----------------------------
INSERT INTO `detectioninfo` VALUES ('25', '20130627', '1002', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '3', '毫克', 'abc123', '方便面', '河北', 'sdf', '李四', '2013-05-28 00:00:00', 'sdf', '合格', '0.6', 'GB 2760-2011', '23', '1233', null);
INSERT INTO `detectioninfo` VALUES ('26', '20130627', '1003', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '3', '毫克', 'abc124', '罐头', '北京', 'sdf', '张三', '2013-05-28 00:00:00', 'sdf', '不合格', '2.6', 'GB 2760-2011', '34', '1234', null);
INSERT INTO `detectioninfo` VALUES ('27', '20130627', '1004', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '3', '毫克', 'ABC234', '火腿肠', '河南', 'sdf', '张三', '2013-06-17 00:00:00', 'sdf', '合格', '2.4', 'GB 2760-2011', '26', '1233', null);
INSERT INTO `detectioninfo` VALUES ('28', '20130627', '1005', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '4', '毫克', 'abc12', '水果', '唐山', 'sdf', '李四', '2013-06-05 00:00:00', 'sdf', '合格', '1.5', 'GB 2760-2011', '1', '1236', null);
INSERT INTO `detectioninfo` VALUES ('29', '20130627', '1006', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '1', '毫克', 'abc126', '罐头', '河北', 'sdf', '张三', '2013-06-17 00:00:00', 'sdf', '不合格', '4.0', 'GB 2760-2012', '0', '1237', null);
INSERT INTO `detectioninfo` VALUES ('30', '20130627', '1007', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '2', '毫克', null, '白菜', '唐山', 'sdf', null, '2013-06-17 00:00:00', 'sdf', '不合格', '2.0', 'GB 2760-2011', '1', '1234', null);
INSERT INTO `detectioninfo` VALUES ('31', '20130627', '1008', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '4', '毫克', null, null, null, 'sdf', null, '2013-04-18 00:00:00', 'sdf', '合格', '2.0', 'GB 2760-2011', '23', '1235', null);
INSERT INTO `detectioninfo` VALUES ('32', '20130627', '1009', '20130703_20130627029_2001_16-46-22.avi', '二氧化硫', '水果干制品', '3', '毫克', null, null, null, 'sdf', null, '2013-03-17 00:00:00', 'sdf', '合格', '2.9', 'GB 2760-2011', '3', '1236', null);
INSERT INTO `detectioninfo` VALUES ('62', '20120423', '3001', null, '二氧化硫', '水果干制品', '3', 'mg/kg', '3', '003', '003', 's', '003', '2013-08-19 11:17:37', 's', '合格', '0', 'GB 2760-2011', '-0.08', '0', null);
INSERT INTO `detectioninfo` VALUES ('63', '20120423', '1001', null, '二氧化硫', '水果干制品', '1', 'mg/kg', '1', '1', '1', 'sdf', '1', '2013-08-16 11:32:21', 'sdf', '合格', '0', 'GB 2760-2011', '-0.389', '0', null);
INSERT INTO `detectioninfo` VALUES ('64', '20120423', '2001', null, '二氧化硫', '水果干制品', '2', 'mg/kg', '2', '2', '2', 's', '2', '2013-08-19 15:11:51', 's', '合格', '0', 'GB 2760-2011', '-0.079', '0', null);
INSERT INTO `detectioninfo` VALUES ('65', '20120423', '3001', null, '二氧化硫', '水果干制品', '3', 'mg/kg', '7', '7', '7', 's', '7', '2013-08-20 15:01:23', 's', '合格', '1', 'GB 2760-2011', '0.019', '0', null);

-- ----------------------------
-- Table structure for `detectioninfos`
-- ----------------------------
DROP TABLE IF EXISTS `detectioninfos`;
CREATE TABLE `detectioninfos` (
  `Info_Id` int(11) NOT NULL AUTO_INCREMENT,
  `p_name` char(100) DEFAULT NULL,
  `type` char(50) DEFAULT NULL,
  `channel` int(11) DEFAULT NULL,
  `result_unit` char(50) DEFAULT NULL,
  `sample_no` char(50) DEFAULT NULL,
  `sample_name` char(100) DEFAULT NULL,
  `sites` char(100) DEFAULT NULL,
  `submission_unit` char(100) DEFAULT NULL COMMENT '送检单位',
  `test_operator` char(50) DEFAULT NULL,
  `test_time` datetime DEFAULT NULL,
  `test_unit` char(100) DEFAULT NULL,
  `Juge_result` char(100) DEFAULT NULL,
  `Detect_result` char(100) DEFAULT NULL,
  `standard` char(50) DEFAULT NULL,
  `Absolut_result` char(50) DEFAULT NULL,
  `Device_ID` int(11) DEFAULT NULL,
  `Is_Quli` int(11) DEFAULT NULL,
  PRIMARY KEY (`Info_Id`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of detectioninfos
-- ----------------------------
INSERT INTO `detectioninfos` VALUES ('25', '测试工程1', '面食', '3', '毫克', 'abc123', '方便面', '河北', '康师傅集团', '李四', '2013-05-28 00:00:00', '中科方德软件有限公司', '合格', '0.6', '0.9', '23', '1233', null);
INSERT INTO `detectioninfos` VALUES ('26', '测试工程2', '罐头', '3', '毫克', 'abc124', '罐头', '北京', '统一', '张三', '2013-05-28 00:00:00', '中科方德', '不合格', '1.6', '1.4', '34', '1234', null);
INSERT INTO `detectioninfos` VALUES ('27', '测试工程', '干果', '3', '毫克', 'ABC234', '火腿肠', '河南', '双汇火腿肠', '张三', '2013-06-17 00:00:00', '中科方德', '合格', '2.4', '2.9', '26', '1233', null);
INSERT INTO `detectioninfos` VALUES ('28', '测试', '食品', '4', '毫克', 'abc12', '水果', '唐山', '农夫山泉', '李四', '2013-06-05 00:00:00', '中科方德软件有限公司', '合格', '0.5', '0.9', '0.8', '1236', null);
INSERT INTO `detectioninfos` VALUES ('29', '测试3', '水果', '1', '毫克', 'abc126', '罐头', '河北', '双汇火腿肠', '张三', '2013-06-17 00:00:00', '中科方德', '不合格', '1.0', '0.2', '0.5', '1237', null);
INSERT INTO `detectioninfos` VALUES ('30', '测试2', '蔬菜', '2', '毫克', null, '白菜', '唐山', '统一', null, '2013-06-17 00:00:00', '中科软', '不合格', '2.0', '1.2', '1.3', '1234', null);
INSERT INTO `detectioninfos` VALUES ('31', '白菜', '蔬菜', '4', '毫克', null, null, null, '农夫山泉', null, '2013-04-18 00:00:00', null, '合格', '2.0', '2.3', '23', '1235', null);
INSERT INTO `detectioninfos` VALUES ('32', '测试3', '食品', '3', '毫克', null, null, null, '农夫山泉', null, '2013-03-17 00:00:00', null, '合格', '2.9', '3.6', '3', '1236', null);
INSERT INTO `detectioninfos` VALUES ('33', '测试3', '食品', '2', '毫克', null, '罐头', null, '农夫山泉', null, '2013-04-17 00:00:00', null, '不合格', '3.9', '3.4', '12', '1234', null);
INSERT INTO `detectioninfos` VALUES ('34', '测试3', '食品', '1', '毫克', null, '罐头', null, '统一', null, '2013-03-17 00:00:00', null, '合格', '0.6', '0.8', '13', '1233', null);

-- ----------------------------
-- Table structure for `device`
-- ----------------------------
DROP TABLE IF EXISTS `device`;
CREATE TABLE `device` (
  `Device_ID` int(11) NOT NULL AUTO_INCREMENT,
  `Device_PassWord` char(30) DEFAULT NULL,
  `Device_Code` char(10) DEFAULT '1',
  `Sockets` char(255) DEFAULT NULL,
  `Device_Group_ID` int(255) DEFAULT NULL,
  `Company_Group_ID` int(11) DEFAULT NULL,
  `Device_Name` char(50) DEFAULT NULL,
  `Device_Addr` char(50) DEFAULT NULL,
  `Device_Type` char(50) DEFAULT NULL,
  `Device_Ver` char(50) DEFAULT NULL,
  `Device_Level` tinyint(4) DEFAULT NULL,
  `Device_Status` int(11) DEFAULT NULL,
  `Device_X` int(11) DEFAULT '10',
  `Device_Y` int(11) DEFAULT '10',
  `Resolution` char(50) DEFAULT NULL,
  `Frames` int(11) DEFAULT NULL,
  `Bit_Rate` int(11) DEFAULT NULL,
  `Form` char(10) DEFAULT NULL,
  `Pic_Width` int(11) DEFAULT NULL,
  `Pic_Heigt` int(11) DEFAULT NULL,
  PRIMARY KEY (`Device_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=1241 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of device
-- ----------------------------
INSERT INTO `device` VALUES ('1233', '1234', '1', '', '0', '4', '第8检测点', '昌平', 'T09', 'V1.0', null, '0', '196', '291', '320*240', '25', '400', 'avi', '320', '320');
INSERT INTO `device` VALUES ('1234', '1234', '1', '192.168.10.230:44858', '1', '4', '第3检测点', '昌平', 'T10', 'V1.0', null, '1', '592', '226', '320*240', '25', '400', 'avi', '320', '320');
INSERT INTO `device` VALUES ('1235', '1235', '1', '192.168.10.240:52503', '2', '5', '第2检测点', '昌平', 'T09', 'V1.8', null, '1', '417', '186', '320*240', '25', '400', 'avi', '320', '240');
INSERT INTO `device` VALUES ('1237', '1234', '1', '', '1', '4', '第4监控点', '昌平', 'v42', 'V1.5', '0', '0', '328', '48', '320*240', '5', '400', 'avi', null, null);
INSERT INTO `device` VALUES ('1240', null, '1', null, '0', null, '第9', '', '', '', null, null, '325', '169', null, null, null, null, null, null);

-- ----------------------------
-- Table structure for `devicegroup`
-- ----------------------------
DROP TABLE IF EXISTS `devicegroup`;
CREATE TABLE `devicegroup` (
  `Device_Group_ID` int(11) NOT NULL AUTO_INCREMENT,
  `Project_Name` char(30) DEFAULT NULL,
  `Is_Squre` int(11) DEFAULT '0',
  PRIMARY KEY (`Device_Group_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of devicegroup
-- ----------------------------
INSERT INTO `devicegroup` VALUES ('1', '海淀监测点', '1');
INSERT INTO `devicegroup` VALUES ('2', '房山区监测点', '1');
INSERT INTO `devicegroup` VALUES ('3', '朝阳监测点', '1');
INSERT INTO `devicegroup` VALUES ('4', '农残多企业', '0');
INSERT INTO `devicegroup` VALUES ('7', '大兴监测点', '1');

-- ----------------------------
-- Table structure for `foodtestlist`
-- ----------------------------
DROP TABLE IF EXISTS `foodtestlist`;
CREATE TABLE `foodtestlist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` char(200) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of foodtestlist
-- ----------------------------

-- ----------------------------
-- Table structure for `ftinfor`
-- ----------------------------
DROP TABLE IF EXISTS `ftinfor`;
CREATE TABLE `ftinfor` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ftestitems` char(50) CHARACTER SET utf8 DEFAULT NULL,
  `fsample` char(100) CHARACTER SET utf8 DEFAULT NULL,
  `wavemode` char(4) CHARACTER SET utf8 DEFAULT NULL,
  `wavemajor` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `waveminor` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `formulaC` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `formulaB` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `formulaA` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `interdilute` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `unit` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `standards` char(50) CHARACTER SET utf8 DEFAULT NULL,
  `comparemode` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `comparemin` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `comparemax` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `testrangemax` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `testrangemin` char(10) CHARACTER SET utf8 DEFAULT NULL,
  `alarm_red` char(10) DEFAULT NULL COMMENT '红色报警',
  `alarm_pitch` char(10) DEFAULT NULL COMMENT '粉色报警',
  `alarm_yellow` char(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of ftinfor
-- ----------------------------
INSERT INTO `ftinfor` VALUES ('1', '二氧化硫', '水果干制品', '1', '1', '1', '1', '1', '1', '3', 'mg/kg', 'GB 2760-2011', '1', '0.3', '1.6', '5', '0.1', '0.9', '0.7', '0.4');
INSERT INTO `ftinfor` VALUES ('4', '测试', '册数', '', '', '', '', '', '', '', '', 'GB 2760-2012', '', '', '1.2', '2', '', '0.9', '0.7', '0.4');

-- ----------------------------
-- Table structure for `paperwork`
-- ----------------------------
DROP TABLE IF EXISTS `paperwork`;
CREATE TABLE `paperwork` (
  `batch_id` char(255) DEFAULT NULL,
  `task_id` char(255) DEFAULT NULL,
  `photo_path` char(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of paperwork
-- ----------------------------

-- ----------------------------
-- Table structure for `picinfo`
-- ----------------------------
DROP TABLE IF EXISTS `picinfo`;
CREATE TABLE `picinfo` (
  `Picinfo_ID` int(11) NOT NULL AUTO_INCREMENT,
  `Device_ID` int(11) DEFAULT NULL,
  `Pic_Name` char(100) DEFAULT NULL,
  `Pic_Info` mediumblob,
  `test_time` datetime DEFAULT NULL,
  PRIMARY KEY (`Picinfo_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of picinfo
-- ----------------------------
INSERT INTO `picinfo` VALUES ('1', '1234', '20130703_20130627029_2001_222_ccc_16-46-08.jpg', null, null);
INSERT INTO `picinfo` VALUES ('2', '1235', '20130703_20130627029_2001_222_ccc_16-46-08.jpg', null, null);
INSERT INTO `picinfo` VALUES ('6', '1234', '20130703_20130627029_2001_222_ccc_16-46-08.jpg', null, '2013-08-23 14:24:58');
INSERT INTO `picinfo` VALUES ('7', '1235', '20130821_20120423150_3001___18-30-28.jpg', null, '2013-08-23 16:26:40');

-- ----------------------------
-- Table structure for `position`
-- ----------------------------
DROP TABLE IF EXISTS `position`;
CREATE TABLE `position` (
  `Positon_ID` int(11) NOT NULL AUTO_INCREMENT,
  `User_ID` int(11) DEFAULT NULL,
  `Dns_X` int(11) DEFAULT NULL,
  `Sns_Y` int(11) DEFAULT NULL,
  `Device_ID` int(11) DEFAULT NULL,
  PRIMARY KEY (`Positon_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of position
-- ----------------------------

-- ----------------------------
-- Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `User_ID` int(11) NOT NULL,
  `PassWord` char(30) DEFAULT NULL,
  `City` char(50) DEFAULT NULL,
  `Company` char(50) DEFAULT NULL,
  `Mobile` char(12) DEFAULT NULL,
  `Is_Admin` int(1) DEFAULT NULL,
  `Desc` char(255) DEFAULT NULL,
  `User_Name` char(30) DEFAULT NULL,
  `Email` char(50) DEFAULT NULL,
  `CenterCode` char(10) DEFAULT NULL,
  PRIMARY KEY (`User_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('12', '222', '', 's', '', null, null, null, '222', '');
INSERT INTO `user` VALUES ('1234', '1234', '北京', 'sdf', '136210', null, null, '张三', null, '1');
DROP TRIGGER IF EXISTS `t_afterinsert_on_tab1`;
DELIMITER ;;
CREATE TRIGGER `t_afterinsert_on_tab1` AFTER INSERT ON `devicegroup` FOR EACH ROW if (new.Is_Squre>0)
then
insert into device(Company_Group_ID) values(new.Device_Group_ID);
else
insert into device(Device_Group_ID) values(new.Device_Group_ID);
end if
;;
DELIMITER ;
DROP TRIGGER IF EXISTS `t_afterupdate_on_tab1`;
DELIMITER ;;
CREATE TRIGGER `t_afterupdate_on_tab1` AFTER UPDATE ON `devicegroup` FOR EACH ROW if (new.Is_Squre>0)
then update device set device.Company_Group_ID=new.Device_Group_ID  where device.Company_Group_ID=old.Device_Group_ID;
else
update device set device.Device_Group_ID=new.Device_Group_ID  where device.Device_Group_ID=old.Device_Group_ID;
end if
;;
DELIMITER ;
DROP TRIGGER IF EXISTS `t_afterdelete_on_tab1`;
DELIMITER ;;
CREATE TRIGGER `t_afterdelete_on_tab1` AFTER DELETE ON `devicegroup` FOR EACH ROW if (old.Is_Squre>0)
then
delete from device where device.Company_Group_ID=old.Device_Group_ID;
else
delete from device where device.Device_Group_ID=old.Device_Group_ID;
end if
;;
DELIMITER ;
