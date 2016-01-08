/**
 * Default GTA:IV Pickups (Health, Armor, Weapons) extracted from the game sco files
 *
 * Author: mabako
 */

local pickups = [];
local function GetWeaponPickupModel(a)
{
	switch(a)
	{
		case 1:
			return 0x68d19067;
		case 2:
			return 0x4e746e14;
		case 3:
			return 0x3dfebd8b;
		case 4:
			return 0x3b373591;
		case 5:
			return 0x88b44609;
		case 6:
			return 0x5603b9ec; // rpg
		case 7:
			return 0xf44c839d; // w_glock
		case 9:
			return 0x5caeb064; // w_eagle
		case 10:
			return 0x6e10d6c3;
		case 11:
			return 0xddb2b5ad; // w_pumpshot
		case 12:
			return 0xafd2e07b; // w_uzi
		case 13:
			return 0x65daddfd;
		case 14:
			return 0x1bdd0543;
		case 15:
			return 0x35855559; // w_m4
		case 16:
			return 0x08762932; // w_rifle
		case 17:
			return 0x22c755b0; // w_psg1 ?
		default:
			// log("Unknown Weapon " + a);
	}
	return 0;
}

local createPickupFn = createPickup;
local createPickup = function(a,b,c,d,e,f,g,h,i)
{
	if(a==0)
		return;
	d = d.tofloat();
	e = e.tofloat();
	f = f.tofloat();
	g = g.tofloat();
	h = h.tofloat();
	i = i.tofloat();
	
	local id = createPickupFn(a,b,c,d,e,f,g,h,i);
	pickups.append(id);
	return id;
}

addEvent("scriptInit",
	function()
	{
		createPickup(0x3fc62578, 2, 200, 987.08, 1828.512, 19.249, 40.32, 0, 180);
		createPickup(0x3fc62578, 2, 200, 388.18, 1911.6, 18.488, 90, 0, -33.84);
		createPickup(0x3fc62578, 2, 200, 1383.959, 1701.16, 20.38, 0.02, 0, 90);
		createPickup(0x3fc62578, 2, 200, 1193, 1442, 15.814, 90.0, 0, 51.12);
		createPickup(0x3fc62578, 2, 200, 696.717, 1462.75, 15.14, 0.0, 0, 90);
		createPickup(0x3fc62578, 2, 200, 1173.88, 1669.57, 17.687, 90.0, 0, 90);
		createPickup(0x3fc62578, 2, 200, 897.3, -134.0, 5.0, 90, 0, 49.68);
		createPickup(0x3fc62578, 2, 200, 1305.42, 677.472, 37.02, 0.0, 0.0, 0.0);
		createPickup(0x3fc62578, 2, 200, 753, -351, 16.55, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, 850.06, 414.16, 10.554, 96.02, -0.38, 0);
		createPickup(0x3fc62578, 2, 200, 1791.915, 199.18, 21, 0, 0, 65.64);
		createPickup(0x3fc62578, 2, 200, 1849.1, 527.48, 29.44, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, 694.36, -309.81, 9.97, 90.0, 0.0, 0.0);
		createPickup(0x3fc62578, 2, 200, 769.264, -66.358, 5.996, 0.0, 0.0, 15.34);
		createPickup(0x3fc62578, 2, 200, -425.44, 1300.24, 17.64, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, 59.96, 194.54, 14.84, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, -401.94, -990.56, 4.14, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -302.04, 1647.0, 15.509, 90, 0, 90);
		createPickup(0x3fc62578, 2, 200, -423.41, 974.94, 10.44, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -10.96, -377.1, 11.18, 90, 0, 208.8);
		createPickup(0x3fc62578, 2, 200, -9.378, 1313.98, 17.714, 86.82, -2, 90.0);
		createPickup(0x3fc62578, 2, 200, 429.76, -501.12, 16.77, 90, 0, 57.6);
		createPickup(0x3fc62578, 2, 200, -123.92, 473.2, 9.465, 90, 0, 48.96);
		createPickup(0x3fc62578, 2, 200, 624.834, 806.935, 14.581, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -116, 1325.26, 30.56, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, -543.569, 1254, 89.84, 0.0, 0.0, 80.1);
		createPickup(0x3fc62578, 2, 200, 499.1, -56.5, 8.343, 90.0, 0.2, 311.56);
		createPickup(0x3fc62578, 2, 200, 342.64, -223.02, 6, 0.0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -1583.075, 14.93, 10, 0.0, 0.0, 90.0);
		createPickup(0x3fc62578, 2, 200, -1083.578, -590, 4.24, 0.0, 0.0, 90.0);
		createPickup(0x3fc62578, 2, 200, -1079.22, 691.9, 7.592, 90, 0, 159.84);
		createPickup(0x3fc62578, 2, 200, -1567.27, 941.01, 25.775, 90, 0, 77.76);
		createPickup(0x3fc62578, 2, 200, -2073.94, -80.0, 5.815, 90, 61.2, 0);
		createPickup(0x3fc62578, 2, 200, -954.5, 1449.92, 33.14, 0.0, 0.0, 0.0);
		createPickup(0x3fc62578, 2, 200, -1493, 701.22, 22.515, 90, 0, 114.48);
		createPickup(0x3fc62578, 2, 200, -1193.08, 13.87, 3.14, 0.0, 0.0, 0.0);
		createPickup(0x3fc62578, 2, 200, -1232.213, 1089.02, 20.08, 0.0, 0.0, 45.0);
		createPickup(0x3fc62578, 2, 200, -1261.834, 1894.56, 11.16, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -1967.992, -259.998, 14.416, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -1418.3, 593.799, 19.78, 0, 0, 150);
		createPickup(0x3fc62578, 2, 200, -1430.107, -594.342, 3.202, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -921.7, 935.338, 48.591, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, 89.12, 149.705, 14.76, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, 1206.9, 201.09, 33.76, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, -1322.28, 1268.28, 23.66, 0, 0, 40);
		createPickup(0x3fc62578, 2, 200, -1513.965, 395.976, 20.723, 90, 0, 87.48);
		createPickup(0x3fc62578, 2, 200, -46.48, -0.328, -1.4, 0, 0, 315);
		createPickup(0x3fc62578, 2, 200, -53.405, 675.13, -1.48, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -138.933, 1136.02, -0.26, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -355.44, 1076.172, -2.557, 90, 51.12, 33.84);
		createPickup(0x3fc62578, 2, 200, -357.406, 1265.83, 17.39, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -402.18, 1368.62, -5.248, 90, 0, 38.16);
		createPickup(0x3fc62578, 2, 200, -79.79, -487.48, 3.12, 0, 0, 90);
		createPickup(0x3fc62578, 2, 200, -361.21, 695.621, -2.712, -33.84, 0, 0);
		createPickup(0x3fc62578, 2, 200, 86.16, -738.41, -12.38, 0, 0, 119.92);
		createPickup(0x3fc62578, 2, 200, 103.52, -264.826, 1.59, 0, 0, 0);
		createPickup(0x3fc62578, 2, 200, 29.73, -101.712, -1.39, 0, 0, 150);
		createPickup(0x972daa10, 2, 200, 792.365, 1316.74, 25.515, 269.7, 0, 20.16);
		createPickup(0x972daa10, 2, 200, 672.44, 1680.48, 30.86, 269.7, 0, 40.32);
		createPickup(0x972daa10, 2, 200, 253.16, 1722.58, 2, 253.9, 0, 0);
		createPickup(0x972daa10, 2, 200, 1106.12, 487.24, 41.565, 96.5, 0, 0);
		createPickup(0x972daa10, 2, 200, 1434.32, -287.14, 14.522, 269.7, 0, 0);
		createPickup(0x972daa10, 2, 200, 2233.53, 889.84, 4.605, 269.3, 0, 0);
		createPickup(0x972daa10, 2, 200, 944.15, -499.24, 23.188, 269.4, 0, 26.64);
		createPickup(0x972daa10, 2, 200, 1240.93, 373.16, 29.55, 269.3, 0, 53.28);
		createPickup(0x972daa10, 2, 200, 2102.68, 155.36, 6.859, 289.96, -4.94, 0);
		createPickup(0x972daa10, 2, 200, 1341.46, 1089.45, 1.287, 259.58, 5.1, 0);
		createPickup(0x972daa10, 2, 200, 1587.24, 789.53, 28.758, 268.52, 2.5, 0);
		createPickup(0x972daa10, 2, 200, 1943.8, 405.01, 24.555, 269.3, 0, 0);
		createPickup(0x972daa10, 2, 200, 1213.38, -14.96, 34.385, 269.86, 0, 46.8);
		createPickup(0x972daa10, 2, 200, 689.28, -306.94, 22.925, 270.54, 0, 120.24);
		createPickup(0x972daa10, 2, 200, -29, -887, -0.365, 264.96, 0, 26.64);
		createPickup(0x972daa10, 2, 200, 444, 1059, 1.797, 269.3, 0, 66.96);
		createPickup(0x972daa10, 2, 200, 617.167, 718.344, 5.213, 282.42, 16.64, 133.2);
		createPickup(0x972daa10, 2, 200, 412.8, 360.56, 7.692, 269.76, 0, 73.44);
		createPickup(0x972daa10, 2, 200, 64.1, 606.2, 9.365, 267.66, 0, 0);
		createPickup(0x972daa10, 2, 200, -760.84, 452.68, 13.02, 269.3, 0, 40.32);
		createPickup(0x972daa10, 2, 200, -184.328, 587.234, 125.82, 270.28, 0, 0);
		createPickup(0x972daa10, 2, 200, -333.98, 1, 13.837, 269.3, 0, 0);
		createPickup(0x972daa10, 2, 200, -608, -699.74, 12.15, 269.3, 0, 0);
		createPickup(0x972daa10, 2, 200, -577.47, 1544.16, 1.37, 269.3, 0, 259.92);
		createPickup(0x972daa10, 2, 200, 196.5, -525.33, 8.15, 269.3, 0, 113.04);
		createPickup(0x972daa10, 2, 200, -24.29, 1633.22, 6.473, 269.3, 0, 66.96);
		createPickup(0x972daa10, 2, 200, 104.23, 985.57, 13.78, 268.9, 2.6, 253.44);
		createPickup(0x972daa10, 2, 200, -328.25, 351.27, 17.645, 285.52, 0, 90);
		createPickup(0x972daa10, 2, 200, -1745.64, 306, 24.53, 269.3, 0, 166.32);
		createPickup(0x972daa10, 2, 200, -2032.02, -339.74, 3.587, 299.18, 0, 0);
		createPickup(0x972daa10, 2, 200, -1497.12, -277.731, 1.918, 269.3, 0, 313.2);
		createPickup(0x972daa10, 2, 200, -1865.76, -161.78, 2.914, 269.3, 0, 0);
		createPickup(0x972daa10, 2, 200, -964, 1895, 21.375, 0, 0, 273.6);
		createPickup(0x972daa10, 2, 200, -1424.31, 628.01, 35.08, 269.3, 0, 166.32);
		createPickup(0x972daa10, 2, 200, -1784.45, 666.98, 13.469, 258.08, -6.8, 73.44);
		createPickup(0x972daa10, 2, 200, -837.44, 1281.61, 18.718, 278.78, -8.08, 40.32);
		createPickup(0x972daa10, 2, 200, -1356.239, 1692.37, 32.23, 26.64, 0, 100.08);
		createPickup(0x972daa10, 2, 200, -1298.68, 943.94, 18.645, 269.3, 0, 66.96);
		createPickup(0x972daa10, 2, 200, -888.7, 625.51, 2.55, 0, 0, 0);
		createPickup(0x972daa10, 2, 200, -1214.61, 531.44, 13.123, 269.3, 0, 73.44);

		createPickup(GetWeaponPickupModel(1), 15, 0, 1309.508, 1674.358, 17.06, 15.84, 180.0, 0.0)
		createPickup(GetWeaponPickupModel(1), 15, 0, 460.4, 1242.54, 18.38, 15.84, 180.0, 31.68)
		createPickup(GetWeaponPickupModel(3), 15, 0, 664.62, 1522.04, 16.02, 90, 180, 144)
		createPickup(GetWeaponPickupModel(3), 15, 0, 917.24, 1536.78, 16.54, 90, 0, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 1167.96, 1666.66, 17.608, 90.0, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(13), 15, 30, 698.48, 1453.68, 19.26, 270, -11.6, 90)
		createPickup(GetWeaponPickupModel(10), 15, 8, 893.08, 1975.04, 19.4, 90.72, 182.16, -70.56)
		createPickup(GetWeaponPickupModel(4), 15, 3, 636, 1612, 25.312, 90, 30, 30)
		createPickup(GetWeaponPickupModel(5), 15, 3, 1482.38, 1566.58, 0.687, 0.0, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(14), 15, 30, 1213.97, 1398.38, 33.99, 217.44, 253.44, 308.88)
		createPickup(GetWeaponPickupModel(1), 15, 0, 738.94, 1930.88, 26.76, 15.84, 174.24, 213.84)
		createPickup(GetWeaponPickupModel(7), 15, 17, 565.94, 1401.24, 29.869, 89.0, 0.0, 55.44)
		createPickup(GetWeaponPickupModel(15), 15, 30, 1200.883, 1465.944, 15.73, 275.24, 0, 0)
		createPickup(GetWeaponPickupModel(11), 15, 8, 1199.305, 1694.62, 17.03, 90, 246.96, 144.72)
		createPickup(GetWeaponPickupModel(1), 15, 0, 1048.98, -546.66, 13.16, 43.2, 98.64, 7.92)
		createPickup(GetWeaponPickupModel(1), 15, 0, 1807.4, 847.62, 21, 0, 162, 87.12)
		createPickup(GetWeaponPickupModel(1), 15, 0, 959.68, -502.58, 14.264, 0, 92.6, -31.68)
		createPickup(GetWeaponPickupModel(3), 15, 0, 827.56, 39.02, 5.033, 91.24, -185.76, 134.64)
		createPickup(GetWeaponPickupModel(3), 15, 0, 927.02, 354.26, 33.2, -19.44, 162, 43.2)
		createPickup(GetWeaponPickupModel(3), 15, 0, 1393, 217, 25.927, 93.6, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(3), 15, 0, 1183.92, -166.44, 26.91, 95.16, 14.16, 69.8)
		createPickup(GetWeaponPickupModel(7), 15, 17, 957, -268, 36.921, 90, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 997.54, 205.9, 31.12, 87.3, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 914.8, -654.66, 14.989, 100.68, 0, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 1780.92, 425.18, 26.985, 87, 0, 126.72)
		createPickup(GetWeaponPickupModel(9), 15, 9, 1968.1, 658, 32.164, 89.1, 0, 43.2)
		createPickup(GetWeaponPickupModel(12), 15, 50, 976.4, 715.22, 39.555, 74.88, -35.28, 198)
		createPickup(GetWeaponPickupModel(12), 15, 50, 1454.12, 37.54, 24.02, 87, 0, 0)
		createPickup(GetWeaponPickupModel(12), 15, 50, 1871.52, 104, 13.15, 87.04, 8.96, 0.0)
		createPickup(GetWeaponPickupModel(13), 15, 30, 1312.58, 54.58, 36.29, 90.72, 177.28, 11.52)
		createPickup(GetWeaponPickupModel(13), 15, 30, 2221.1, 336.34, 16.27, 271.72, 0, 0)
		createPickup(GetWeaponPickupModel(10), 15, 8, 1668.348, 1180.17, 0.878, 79.2, 241.2, 308.88)
		createPickup(GetWeaponPickupModel(11), 15, 8, 2648.81, 419, 5.24, 87.12, 253.44, 0)
		createPickup(GetWeaponPickupModel(4), 15, 3, 1489.06, 563.6, 28.86, 93, 30, 30)
		createPickup(GetWeaponPickupModel(4), 15, 3, 2122.2, -25.9, 4.816, 90, 30, 30)
		createPickup(GetWeaponPickupModel(4), 15, 3, 1078.34, 859.12, 31.58, 94.32, 30, 60)
		createPickup(GetWeaponPickupModel(5), 15, 3, 1141.96, -697.22, 15.44, 90, 0, 59.76)
		createPickup(GetWeaponPickupModel(5), 15, 3, 767.26, 456, 3.02, 0.0, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(5), 15, 3, 1323.48, -415.98, 17.35, 79.2, 0, 0)
		createPickup(GetWeaponPickupModel(14), 15, 30, 1027.86, 591.92, 33.77, 90.72, -9.36, 82.8)
		createPickup(GetWeaponPickupModel(10), 15, 8, 2108.46, 722.067, 8.746, -91.4, 234, 40)
		createPickup(GetWeaponPickupModel(12), 15, 50, 430.473, 1063.32, 0.682, 87, 0, 60.0)
		createPickup(GetWeaponPickupModel(1), 15, 0, 149.36, 997.48, 3.343, 0, 93.36, 48.96)
		createPickup(GetWeaponPickupModel(1), 15, 0, 468, 402, 7.41, 0, 93.16, -117.36)
		createPickup(GetWeaponPickupModel(3), 15, 0, -478.44, 796.76, 8.988, 90.72, 21.6, 225.36)
		createPickup(GetWeaponPickupModel(3), 15, 0, -235.78, 692.0, 2.447, 88.98, 9.36, 228.96)
		createPickup(GetWeaponPickupModel(7), 15, 17, -820.9, -4.12, 3.845, 87.3, 0, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, -183.0, 1080, 5.677, 87.3, -10.98, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, -75.8, 1574.2, 22.84, -68.36, 10.34, -0.78)
		createPickup(GetWeaponPickupModel(9), 15, 9, -235.7, -480.24, 4.193, 89.52, 0, 0)
		createPickup(GetWeaponPickupModel(9), 15, 9, -31.62, 70.52, 21.507, 89.52, 0.0, 179.28)
		createPickup(GetWeaponPickupModel(9), 15, 9, 592.66, 784.38, 13.284, 87.4, 0, 0)
		createPickup(GetWeaponPickupModel(12), 15, 50, 101.3, -610.8, 13.787, 88.34, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(12), 15, 50, -93.78, 580.2, 15.337, 88.34, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(13), 15, 30, -220.76, 1378.76, 22.12, 90, 180, 0)
		createPickup(GetWeaponPickupModel(13), 15, 30, -533.52, 1356.68, 20.14, 90, 180, 0)
		createPickup(GetWeaponPickupModel(14), 15, 30, 486.3, -85, 8.748, 97.92, 286.56, 0)
		createPickup(GetWeaponPickupModel(14), 15, 30, -141.664, -861.1, 18.649, 90.72, -25.2, 179.28)
		createPickup(GetWeaponPickupModel(15), 15, 30, 180, 260, 25.74, 82.8, 5.76, 0)
		createPickup(GetWeaponPickupModel(15), 15, 30, 66.94, 42.26, 21.03, 51.84, 252, 13.68)
		createPickup(GetWeaponPickupModel(10), 15, 8, -118.34, 1151.64, -1.47, 90.0, 0.0, 51.84)
		createPickup(GetWeaponPickupModel(10), 15, 8, -637.437, 1137.8, 9.265, 85.52, 246.24, 16.56)
		createPickup(GetWeaponPickupModel(11), 15, 8, 106, 1135, 20.755, 97.92, 0, 0)
		createPickup(GetWeaponPickupModel(11), 15, 8, 167, -915, 8.255, 102.24, 0, 48.24)
		createPickup(GetWeaponPickupModel(5), 15, 3, -415.36, 1842.22, 3.62, 0, 0, 0)
		createPickup(GetWeaponPickupModel(5), 15, 3, -723.66, 357, 9.575, 0, 0, 0)
		createPickup(GetWeaponPickupModel(4), 15, 3, 73, -448, 19.728, 94.62, 30, 60)
		createPickup(GetWeaponPickupModel(4), 15, 3, -485, -213, 13.86, 94.62, 30, 60)
		createPickup(GetWeaponPickupModel(18), 15, 3, -194.66, 1866.16, 8.385, -86.4, 9.14, 5.76)
		createPickup(GetWeaponPickupModel(17), 15, 7, -221.84, 1735.42, 48.159, -9.36, -120.96, 0)
		createPickup(GetWeaponPickupModel(16), 15, 7, 162.44, 432.64, 31.94, 90.0, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(4), 15, 3, -39.82, 111, 13.755, 94.62, 40.32, 0)
		createPickup(GetWeaponPickupModel(4), 15, 3, -354, -2, 13.545, 88.5, 45, 0)
		createPickup(GetWeaponPickupModel(4), 15, 3, -278, 294, 13.869, 94.62, 45.72, 59.76)
		createPickup(GetWeaponPickupModel(4), 15, 3, -478, 27, 8.017, 94.62, 45.72, 133.2)
		createPickup(GetWeaponPickupModel(1), 15, 0, -1629.32, 730.595, 27.978, 20.88, 151.92, -32.4)
		createPickup(GetWeaponPickupModel(3), 15, 0, -2018.0, 136.0, 11.263, 41.12, 0, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, -1553.4, 477.34, 25.347, 90, 0, -48.24)
		createPickup(GetWeaponPickupModel(7), 15, 17, -1265.34, 1693.98, 31.865, 89.08, -17.28, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, -665.46, 1489, 5.175, 96.36, -6.46, -5.76)
		createPickup(GetWeaponPickupModel(7), 15, 17, -1221, 1870, 10.526, 87, 0, 0)
		createPickup(GetWeaponPickupModel(9), 15, 9, -929.66, 652.54, 4.442, 88.82, 0, 0)
		createPickup(GetWeaponPickupModel(9), 15, 9, -1255, -486, 22.728, 88.82, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(12), 15, 50, -1019.5, 1675, 23.96, 82, 5.1, 20.16)
		createPickup(GetWeaponPickupModel(12), 15, 50, -1283, 710, 18.962, 87.92, 0.0, 0.0)
		createPickup(GetWeaponPickupModel(13), 15, 30, -1102.38, 414.32, 26.392, 90, 180, -40.32)
		createPickup(GetWeaponPickupModel(13), 15, 30, -1469.62, -595, 64.34, 90, 180, 0)
		createPickup(GetWeaponPickupModel(13), 15, 30, -1223.42, 1313.68, 21.188, 90, 180.0, 0.0)
		createPickup(GetWeaponPickupModel(13), 15, 30, -1074.94, 1197.86, 16.922, 90, 180, 28.8)
		createPickup(GetWeaponPickupModel(15), 15, 30, -1525.9, 1391.64, 1.24, 0.0, 67.68, 0.0)
		createPickup(GetWeaponPickupModel(15), 15, 30, -1745, -62.2, 57.82, 97.92, 236.88, 86.4)
		createPickup(GetWeaponPickupModel(15), 15, 30, -1509.24, 183.887, 11.932, 86.4, 244.8, 94.32)
		createPickup(GetWeaponPickupModel(10), 15, 8, -1524.15, 1121.22, 25.526, 97.02, 179.62, 264.76)
		createPickup(GetWeaponPickupModel(11), 15, 8, -980.5, 1040.08, 33.367, 83.5, 0, 40.32)
		createPickup(GetWeaponPickupModel(11), 15, 8, -749.62, 1161.94, 0.54, 204.48, -60.48, -17.28)
		createPickup(GetWeaponPickupModel(11), 15, 8, -1585.615, -284.035, -3.573, 56.16, 239.04, 87.84)
		createPickup(GetWeaponPickupModel(5), 15, 3, -2236.62, 219.824, 11.26, -82.24, 5, 8.64)
		createPickup(GetWeaponPickupModel(5), 15, 3, -1642.2, 290.86, 22.27, 93.16, 0, -55.44)
		createPickup(GetWeaponPickupModel(4), 15, 3, -1200.78, 1058.68, 18.605, 90, 30, 30)
		createPickup(GetWeaponPickupModel(4), 15, 3, -1276, 4, 17.41, 90, 30, 0.0)
		createPickup(GetWeaponPickupModel(18), 15, 3, -1399.28, 986.14, 118.255, -89.28, 2.5, 139.14)
		createPickup(GetWeaponPickupModel(18), 15, 3, -2129.06, -446.74, 5.15, -89.72, -9.36, 139.14)
		createPickup(GetWeaponPickupModel(16), 15, 7, -1900.415, 259.26, 74.552, 51.12, -55.44, 188.64)
		createPickup(GetWeaponPickupModel(16), 15, 7, -1832.44, -590.184, 39.5, 24.48, -70.56, 108.72)
		createPickup(GetWeaponPickupModel(14), 15, 30, -1404.46, 547.15, 19.05, -51.12, 245.52, 105.12)
		createPickup(GetWeaponPickupModel(16), 15, 7, -1275.18, 4.538, 10.645, -86.4, -107.28, 271.44)
		createPickup(GetWeaponPickupModel(1), 15, 0, 654.75, 281, 35.115, 32.4, 178.14, 259.26)
		createPickup(GetWeaponPickupModel(1), 15, 0, 149, 1639, 25.88, 0, 93.36, 0)
		createPickup(GetWeaponPickupModel(3), 15, 0, 518, 709, 18.718, 87.48, -13.68, 82.08)
		createPickup(GetWeaponPickupModel(3), 15, 0, 94.82, 1715.3, 28.747, 89.28, 13.68, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 651.78, 63.3, 9.172, 88.8, 0, 0)
		createPickup(GetWeaponPickupModel(7), 15, 17, 642.89, -408.03, 45.241, 89.04, 0, 128.88)
		createPickup(GetWeaponPickupModel(9), 15, 9, 890.9, 1404.32, 24.288, 87, 0, 0)
		createPickup(GetWeaponPickupModel(12), 15, 50, -807.52, -756.0, 4.26, 87.82, 0, 0)
		createPickup(GetWeaponPickupModel(12), 15, 50, -632.72, 663.3, -26.22, 92.98, 8.62, 0.0)
		createPickup(GetWeaponPickupModel(4), 15, 3, -603.52, -794.2, 3.873, 90, 30, 0)
		createPickup(GetWeaponPickupModel(5), 15, 3, 552.62, 1121.36, 31.065, 0, 0, 0)
		createPickup(GetWeaponPickupModel(18), 15, 3, 274.48, -892.98, 3.94, 82.08, 1.7, 0)
		createPickup(GetWeaponPickupModel(11), 15, 8, 511.943, -47.181, 8.55, 65.52, 245.52, -7.2)
		createPickup(GetWeaponPickupModel(4), 15, 3, 657.46, 736.86, 14.122, 90, 35, 48.24)
	}
);

addEvent("scriptExit",
	function()
	{
		foreach(id, pickup in pickups)
		{
			deletePickup(id);
		}
	}
);
