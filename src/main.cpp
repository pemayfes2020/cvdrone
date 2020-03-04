#include "ardrone/ardrone.h"

// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------
const int START = 0;
const int FIND_OBJECT = 1;
const int ROTATE = 2;
const int GO_TOWARDS = 3;
const int ARRIVED = 4;


int LOW_HUE;           //hueの下限
int UP_HUE;			   //hueの上限


int main(int argc, char *argv[])
{
	// AR.Drone class
	ARDrone ardrone;

	// Initialize
	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// // Instructions
	// std::cout << "***************************************" << std::endl;
	// std::cout << "*       CV Drone sample program       *" << std::endl;
	// std::cout << "* - Controls -                        *" << std::endl;
	// std::cout << "*    'Space' -- Takeoff/Landing       *" << std::endl;
	// std::cout << "*    'Up'    -- Move forward          *" << std::endl;
	// std::cout << "*    'Down'  -- Move backward         *" << std::endl;
	// std::cout << "*    'Left'  -- Turn left             *" << std::endl;
	// std::cout << "*    'Right' -- Turn right            *" << std::endl;
	// std::cout << "*    'Q'     -- Move upward           *" << std::endl;
	// std::cout << "*    'A'     -- Move downward         *" << std::endl;
	// std::cout << "*    'B'     -- auto-up-and-down      *" << std::endl;
	// std::cout << "* - Others -                          *" << std::endl;
	// std::cout << "*    'C'     -- Change camera         *" << std::endl;
	// std::cout << "*    'Esc'   -- Exit                  *" << std::endl;
	// std::cout << "***************************************" << std::endl;


	// // Battery
	std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;


	double target_x, target_y, target_z;
	int phase = START;

	double pre_alt = 0;
	int pre_radius = 0;

	int go_count = 0;

	//はじめは青

	if(argc == 2){
		char* c = argv[1];

		switch (c[0])
		{
		case 'r':
			LOW_HUE = 162;
			UP_HUE = 180;
			break;

		case 'b':
			LOW_HUE = 95;           //hueの下限
			UP_HUE = 105;              //hueの上限
			break;

		case 'y':
			LOW_HUE = 22;           //hueの下限
			UP_HUE = 32;              //hueの上限
			break;

		default:
			break;
		}

	}else{
		LOW_HUE = 95;           //hueの下限
		UP_HUE = 105; 
	}

	int pre_phase = -1;

	std::vector<std::pair<cv::Point,double>> r;


	while (1) {

		// Key input
		int key = cv::waitKey(5);
		if (key == 0x1b) break;

		// Get an image
		cv::Mat image = ardrone.getImage();

		//飛ぶと危ないのでコメントアウトしてる
		//Take off / Landing 
		if(key == ' ') {
			if (ardrone.onGround()) ardrone.takeoff();
			else ardrone.landing();
		}

	
		//phaseごとに分ける

		
		double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;

		switch (phase){
			case START:{//0
				r = ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE);

				if(abs(pre_alt - ardrone.getAltitude()) < 0.05 && pre_alt){
					phase = FIND_OBJECT;//FIND_OBJECTへ
				}
				pre_alt = ardrone.getAltitude();
			}

				break;
			
			case FIND_OBJECT:{  //1
				//その場で回転
				vr = 0.5;

				r = ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE);

				if(r.size() == 1){
					if(abs(r[0].second - pre_radius) < 0.3 && r[0].first.x > 200) phase = ROTATE;
					pre_radius = r[0].second;
				}
			}
				break;
			
			case ROTATE: {//2
				vr = -0.5;
				r = ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE);

				if(r.size() == 1 && r[0].first.x < 400){
					phase = GO_TOWARDS;
				}
			}
				break;
			
			case GO_TOWARDS: {//3
				vx = 0.5;
				go_count++;
				r = ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE);
				
				if(r.size() == 1){
					std::cout << r[0].second << std::endl;

					if(r[0].second > 15){
						phase = ARRIVED;
					}
				}
			}

				break;
			
			case ARRIVED:{
				ardrone.landing();
				cv::imshow("image", image);
			}
				break;
			
			default:{
				cv::imshow("image", image);
			}
				break;
			
		}

		
		ardrone.move3D(vx, vy, vz, vr);

		if(pre_phase != phase){
			std::cout << "phase = " << phase << std::endl;
			pre_phase = phase;
		}

		switch (key)
		{
		case 'r':
			LOW_HUE = 162;
			UP_HUE = 180;
			phase = FIND_OBJECT;//FIND_OBJECTへ

			break;

		case 'b':
			LOW_HUE = 95;           //hueの下限
			UP_HUE = 105;              //hueの上限
			phase = FIND_OBJECT;//FIND_OBJECTへ
			break;

		case 'y':
			LOW_HUE = 22;           //hueの下限
			UP_HUE = 32;              //hueの上限
			phase = FIND_OBJECT;//FIND_OBJECTへ
			break;

		default:
			break;
		}

		// // Move
		// double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
		// switch (key) {
		// case CV_VK_UP:		vx = 1.0; break;
		// case CV_VK_DOWN:	vx = -1.0; break;
		// case CV_VK_LEFT:	vy = 1.0; break;
		// case CV_VK_RIGHT:	vy = -1.0; break;
		// case 'j':	vy = 1.0; break;
		// case 'l':	vy = -1.0; break;
		// case 'q':	vz = 1.0; break;
		// case 'a':	vz = -1.0; break;
		// default:
		// 	break;
		// }
		//ardrone.move3D(vx, vy, vz, vr);

		// Change camera
		static int mode = 0;
		if (key == 'c') ardrone.setCamera(++mode % 4);


		//標準出力で中心座標、半径を確認
		//drawCirlcles(ardrone.color_tracking(image, target_x, target_y, target_z));
		//drawCirlcles(ardrone.detectCircle(image, target_x, target_y, target_z, LOW_HUE, UP_HUE));

		
		if (ardrone.getBatteryPercentage() < 10) {
			std::cout << "Battery low !" << std::endl;
			ardrone.landing();
			break;
		}
	}

	// See you
	ardrone.close();

	return 0;
}

