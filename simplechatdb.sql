/*
 Navicat Premium Data Transfer

 Source Server         : mySql56
 Source Server Type    : MySQL
 Source Server Version : 50645
 Source Host           : localhost:3306
 Source Schema         : simplechatdb

 Target Server Type    : MySQL
 Target Server Version : 50645
 File Encoding         : 65001

 Date: 12/05/2022 10:19:11
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for chatgroup
-- ----------------------------
DROP TABLE IF EXISTS `chatgroup`;
CREATE TABLE `chatgroup`  (
  `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `data_type` varchar(16) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '接口状态码',
  `GroupHeadPic` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '群头像',
  `GroupOwnerID` int(10) UNSIGNED NULL DEFAULT NULL COMMENT '群所有者id',
  `members` longtext CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '群成员json',
  `nickName` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '' COMMENT '群名称',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `GroupOwnerID`(`GroupOwnerID`) USING BTREE,
  CONSTRAINT `chatgroup_ibfk_1` FOREIGN KEY (`GroupOwnerID`) REFERENCES `user` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Table structure for chatgroup_relation
-- ----------------------------
DROP TABLE IF EXISTS `chatgroup_relation`;
CREATE TABLE `chatgroup_relation`  (
  `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `group_id` bigint(20) UNSIGNED NOT NULL DEFAULT 0 COMMENT '群id',
  `user_id` int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '成员id',
  `created_at` timestamp(0) NULL DEFAULT NULL COMMENT '加群时间',
  `updated_at` timestamp(0) NULL DEFAULT NULL COMMENT '退群时间',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `userid`(`user_id`) USING BTREE,
  INDEX `group_id`(`group_id`) USING BTREE,
  CONSTRAINT `userid` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `chatgroup_relation_ibfk_1` FOREIGN KEY (`group_id`) REFERENCES `chatgroup` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Table structure for friend_relation
-- ----------------------------
DROP TABLE IF EXISTS `friend_relation`;
CREATE TABLE `friend_relation`  (
  `id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
  `userid` int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '用户-id',
  `friend_id` int(10) UNSIGNED NOT NULL DEFAULT 0 COMMENT '好友-id',
  `is_out` tinyint(1) UNSIGNED NOT NULL DEFAULT 0 COMMENT '0单向,1=双向',
  `addFriTime` timestamp(0) NULL DEFAULT NULL COMMENT '加好友时间',
  `deleteFriTime` timestamp(0) NULL DEFAULT NULL COMMENT '删除好友时间',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `friend_id`(`friend_id`) USING BTREE,
  INDEX `wx_id`(`userid`) USING BTREE,
  CONSTRAINT `friend_relation_ibfk_1` FOREIGN KEY (`userid`) REFERENCES `user` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `friend_relation_ibfk_2` FOREIGN KEY (`friend_id`) REFERENCES `user` (`id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci COMMENT = 'wx好友关系表' ROW_FORMAT = Compact;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `createtime` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP(0),
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `username`(`username`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Compact;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (1, '1', '1', '2022-04-24 16:25:53');
INSERT INTO `user` VALUES (6, 'ada', '123', '2022-05-11 15:25:34');

SET FOREIGN_KEY_CHECKS = 1;
