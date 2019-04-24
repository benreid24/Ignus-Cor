#include "Shared/Maps/Weather.hpp"
#include "Shared/Maps/Map.hpp"
#include "Shared/Properties.hpp"
#include "Shared/Util/Util.hpp"
#include <cmath>
using namespace std;
using namespace sf;

Clock Weather::timer;

BaseWeatherType::BaseWeatherType(Map* o) {
	owner = o;
}

void BaseWeatherType::update()
{
    //do nothing
}

bool BaseWeatherType::isFinished()
{
    return true;
}

void BaseWeatherType::stop()
{
    //do nothing
}

void BaseWeatherType::draw(RenderTarget& w)
{
    //do nothing
}

int BaseWeatherType::getLightChange()
{
    return 0;
}

void BaseWeatherType::setOwner(Map* o) {
	owner = o;
}

Thunder::Thunder() : flash(Vector2f(800,600))
{
    goTime = 0;
    lastTime = 0;
    flash.setFillColor(Color::Transparent);
}

void Thunder::update(SoundEngine* sEngine)
{
    if (goTime!=0)
    {
        int t = Weather::timer.getElapsedTime().asMilliseconds()-goTime;
        const int a = 70, c = 400, d = 750;
        int alpha = 0;

        if (t>=d)
            goTime = 0;
        else
        {
            if (t<175)
                alpha = -100*t*(t-2*2)/(a*a);
            else
                alpha = 230*(t-d)*(t-2*c+d)/(-c*c+2*c*d-d*d);
        }
        flash.setFillColor(Color(255,255,255,alpha));
    }
    else if (Random(10000,25000)<Weather::timer.getElapsedTime().asMilliseconds()-lastTime)
    {
        sEngine->playSound("thunder.wav");
        flash.setFillColor(Color(255,255,255,128));
        goTime = Weather::timer.getElapsedTime().asMilliseconds();
        lastTime = Weather::timer.getElapsedTime().asMilliseconds();
    }
}

void Thunder::draw(RenderTarget& window)
{
    window.draw(flash);
}

RainWeather::RainWeather(Map* m, SoundEngine* se, bool isHard, bool thunder) : BaseWeatherType(m)
{
    isStopping = isDone = false;
    sEngine = se;
    lastTime = 0;
    creationTimer = Weather::timer.getElapsedTime().asMilliseconds();
    canThunder = thunder;
    airDrop = imagePool.loadResource(Properties::MiscImagePath+"rainDrop.png");
    splashDrop[0] = imagePool.loadResource(Properties::MiscImagePath+"rainSplash1.png");
    splashDrop[1] = imagePool.loadResource(Properties::MiscImagePath+"rainSplash2.png");
    airSpr.setTexture(*airDrop);
    splashSpr[0].setTexture(*splashDrop[0]);
    splashSpr[1].setTexture(*splashDrop[1]);
    airSpr.setOrigin(Vector2f(airDrop->getSize().x/2,airDrop->getSize().y));
    splashSpr[0].setOrigin(Vector2f(splashDrop[0]->getSize().x/2,splashDrop[0]->getSize().y));
    splashSpr[1].setOrigin(Vector2f(splashDrop[1]->getSize().x/2,splashDrop[1]->getSize().y));

    if (isHard)
    {
        sound = sEngine->playSound("hardRain.wav",-1);
        airSpr.setRotation(-45);
        splashSpr[0].setRotation(-45);
        rainVel = Vector3f(0.030303,0.030303,0.757575);
        maxDrops = 1200;
        lChng = 35;
    }
    else
    {
        sound = sEngine->playSound("lightRain.wav",-1);
        airSpr.setRotation(-15);
        splashSpr[1].setRotation(-15);
        splashSpr[0].setRotation(-15);
        rainVel = Vector3f(0,0,0.515151);
        maxDrops = 750;
        lChng = 22;
    }
    createPerSec = 0.303;
    rainDrops.reserve(maxDrops+10);
}

RainWeather::~RainWeather()
{
    sEngine->stopSound(sound);
}

void RainWeather::stop()
{
    isStopping = true;
    sEngine->fadeOut(sound);
}

bool RainWeather::isFinished()
{
    return isDone;
}

int RainWeather::getLightChange()
{
    return lChng;
}

void RainWeather::update() {
    if (isDone) {
		return;
    }

	double dt = Weather::timer.getElapsedTime().asMilliseconds()-lastTime;
	Vector3f dif(rainVel.x*dt,rainVel.y*dt,rainVel.z*dt);
	lastTime = Weather::timer.getElapsedTime().asMilliseconds();

    for (unsigned int i = 0; i<rainDrops.size(); ++i)
    {
        if (rainDrops[i].z!=0)
        {
            rainDrops[i] += dif;
            if (rainDrops[i].z<=0)
                rainDrops[i].z = 0;
        }
        if (rainDrops[i].z==0)
        {
            if (rainDrops[i].time==0)
                rainDrops[i].time = Weather::timer.getElapsedTime().asMilliseconds();
            else if (Weather::timer.getElapsedTime().asMilliseconds()-rainDrops[i].time>=80 && !isStopping)
            {
                int xMin = owner->getCamera().x-300;
                int yMin = owner->getCamera().y-300;
                rainDrops[i] = Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0);
            }
            else if (isStopping)
            {
                rainDrops.erase(rainDrops.begin()+i);
                i--;
            }
        }
    }

	if (Weather::timer.getElapsedTime().asMilliseconds()-creationTimer>100)
    {
    	int created = 0;
		int makeNow = double(Weather::timer.getElapsedTime().asMilliseconds()-creationTimer)*createPerSec;
		creationTimer = Weather::timer.getElapsedTime().asMilliseconds();
		while (signed(rainDrops.size())<maxDrops && created<makeNow)
		{
			int xMin = owner->getCamera().x-300;
			int yMin = owner->getCamera().y-300;
			rainDrops.push_back(Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0));
			created++;
		}
    }

    if (canThunder)
        thunder.update(sEngine);

    if (isStopping)
    {
        maxDrops -= 10;
        if (rainDrops.size()==0)
            isDone = true;
    }
}

void RainWeather::draw(RenderTarget& window)
{
    for (unsigned int i = 0; i<rainDrops.size(); ++i)
    {
        if (rainDrops[i].z>0)
        {
            airSpr.setPosition(rainDrops[i].toScreen(owner->getCamera()+Vector2f(Properties::ScreenWidth,Properties::ScreenHeight)));
            window.draw(airSpr);
        }
        else
        {
            int x = (Weather::timer.getElapsedTime().asMilliseconds()-rainDrops[i].time)/60;
            if (x>1)
                x = 1;
            splashSpr[x].setPosition(rainDrops[i].toScreen(owner->getCamera()+Vector2f(Properties::ScreenWidth,Properties::ScreenHeight)));
            window.draw(splashSpr[x]);
        }
    }
    if (canThunder)
        thunder.draw(window);
}

SnowWeather::SnowWeather(Map* m, SoundEngine* se, bool h, bool t) : BaseWeatherType(m)
{
	sEngine = se;
    isStopping = isDone = false;
    lastTime = 0;
    creationTimer = Weather::timer.getElapsedTime().asMilliseconds();
    flake = imagePool.loadResource(Properties::MiscImagePath+"snowFlake.png");
    spr.setTexture(*flake);
    canThunder = t;
    createPerSec = 0.364;

    if (h)
    {
        fallSpeed = 1.212121;
        maxFlakes = 10000;
        lChng = 30;
        lifeTime = 2000;
    }
    else
    {
        fallSpeed = 0.121212;
        maxFlakes = 6000;
        lChng = 15;
        lifeTime = 4000;
    }
    flakes.reserve(maxFlakes+10);
}

SnowWeather::~SnowWeather()
{
    //dtor
}

bool SnowWeather::isFinished()
{
    return isDone;
}

void SnowWeather::stop()
{
    isStopping = true;
}

int SnowWeather::getLightChange()
{
    return lChng;
}

void SnowWeather::update()
{
    if (isDone) {
        return;
    }

	double dif = double(Weather::timer.getElapsedTime().asMilliseconds()-lastTime)*fallSpeed;
	lastTime = Weather::timer.getElapsedTime().asMilliseconds();

    for (unsigned int i = 0; i<flakes.size(); ++i)
    {
        if (flakes[i].z!=0)
        {
            flakes[i].z -= dif;
            if (flakes[i].z<=0)
            {
                flakes[i].z = 0;
                flakes[i].time = Weather::timer.getElapsedTime().asMilliseconds();
            }
            else
            {
                if (fallSpeed<1) //light
                {
                    flakes[i].x += cos(flakes[i].z*3.1415926/60)*6;
                    flakes[i].y += 2;
                }
                else //hard
                {
                    flakes[i].x += dif/2 + cos(flakes[i].z*3.1415926/60)*12;
                    flakes[i].y += dif/4;
                }
            }
        }
        else
        {
            if (Weather::timer.getElapsedTime().asMilliseconds()-flakes[i].time>lifeTime)
            {
            	if (!isStopping)
                {
                	int xMin = owner->getCamera().x-300;
					int yMin = owner->getCamera().y-300;
					flakes[i] = Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0);
                }
                else if (isStopping)
				{
					flakes.erase(flakes.begin()+i);
					i--;
				}
            }
        }
    }

	if (Weather::timer.getElapsedTime().asMilliseconds()-creationTimer>100)
    {
    	int createNow = double(Weather::timer.getElapsedTime().asMilliseconds()-creationTimer)*createPerSec;
    	int created = 0;
    	creationTimer = Weather::timer.getElapsedTime().asMilliseconds();
		while (signed(flakes.size())<maxFlakes && created<createNow)
		{
			int xMin = owner->getCamera().x-300;
			int yMin = owner->getCamera().y-300;
			flakes.push_back(Vector4D(Random(xMin, xMin+1400),Random(yMin,yMin+1200),Random(50,90),0));
			created++;
		}
    }

    if (canThunder)
        thunder.update(sEngine);
    if (isStopping)
    {
        maxFlakes -= 80;
        if (flakes.size()==0)
            isDone = true;
    }
}

void SnowWeather::draw(RenderTarget& window)
{
    for (unsigned int i = 0; i<flakes.size(); ++i)
    {
        spr.setPosition(flakes[i].toScreen(owner->getCamera()+Vector2f(Properties::ScreenWidth,Properties::ScreenHeight)));
        if (flakes[i].time!=0)
            spr.setColor(Color(255,255,255,255-255*(double(Weather::timer.getElapsedTime().asMilliseconds()-flakes[i].time)/double(lifeTime))));
        else
            spr.setColor(Color(255,255,255,255));
        window.draw(spr);
    }

    if (canThunder)
        thunder.draw(window);
}

SunnyWeather::SunnyWeather() : BaseWeatherType(nullptr),cover(Vector2f(Properties::ScreenWidth,Properties::ScreenHeight))
{
    cover.setFillColor(Color(255,255,60,45));
    t = 0;
    a = 0;
    isStopping = false;
    lastTime = 0;
}

SunnyWeather::~SunnyWeather()
{
    //dtor
}

void SunnyWeather::stop()
{
    isStopping = true;
    a = cover.getFillColor().a;
}

bool SunnyWeather::isFinished()
{
    return cover.getFillColor().a==0;
}

void SunnyWeather::update()
{
    if (!isStopping)
	{
		t += double(Weather::timer.getElapsedTime().asMilliseconds()-lastTime)*0.030303;
		cover.setFillColor(Color(255,255,60,45+cos(double(t)/45*3.1415926)*25));
	}
    else
	{
		a -= 0.0151515*double(Weather::timer.getElapsedTime().asMilliseconds()-lastTime);
		if (a<0)
			a = 0;
		cover.setFillColor(Color(255,255,60,a));
	}
	lastTime = Weather::timer.getElapsedTime().asMilliseconds();
}

void SunnyWeather::draw(RenderTarget& window)
{
    window.draw(cover);
}

int SunnyWeather::getLightChange()
{
    return -20;
}

FogWeather::FogWeather(Map* m, bool isThick) : BaseWeatherType(m)
{
	a = 0;
    isStopping = false;
    lastTime = Weather::timer.getElapsedTime().asMilliseconds();
    spr.setColor(Color(255,255,255,0)); //gradually increase
    fogTxtr = imagePool.loadResource(Properties::MiscImagePath+"fog.png");
    spr.setTexture(*fogTxtr);
    spr.setOrigin(fogTxtr->getSize().x/2-4,fogTxtr->getSize().y/2-4);

    int baseX = owner->getCamera().x-128;
    int baseY = owner->getCamera().y-128;
    for (int x = -7; x<Properties::ScreenWidth*3/20; x++)
    {
        for (int y = -7; y<Properties::ScreenHeight*3/20; y++)
        {
            particles.push_back(Particle(baseX+x*20,baseY+y*20,Random(0,360),double(Random(-5,5))/33));
        }
    }

    if (isThick)
    {
        targetA = 110;
        lChng = 50;
    }
    else
    {
        targetA = 70;
        lChng = 20;
    }
}

FogWeather::~FogWeather()
{
    //dtor
}

void FogWeather::stop()
{
    isStopping = true;
}

bool FogWeather::isFinished()
{
    return spr.getColor().a==0;
}

int FogWeather::getLightChange()
{
    return lChng;
}

void FogWeather::update()
{
	double dt = Weather::timer.getElapsedTime().asMilliseconds()-lastTime;
	lastTime = Weather::timer.getElapsedTime().asMilliseconds();
	double difY = dt*0.030303;
	double difX = difY*2;

    for (unsigned int i = 0; i<particles.size(); ++i)
    {
        particles[i].x += difX;
        particles[i].y += difY;
        particles[i].rotation += particles[i].angularVel*dt;

        if (particles[i].x>owner->getCamera().x+Properties::ScreenWidth*3)
			particles[i] = Particle(owner->getCamera().x-96,particles[i].y,Random(0,360),double(Random(-5,5))/33);
        if (particles[i].y>owner->getCamera().y+Properties::ScreenHeight*3)
			particles[i] = Particle(particles[i].x,owner->getCamera().y-96,Random(0,360),double(Random(-5,5))/33);
    }

    if (a<targetA && !isStopping)
	{
		a += dt*0.060606;
		spr.setColor(Color(255,255,255,a));
	}

    if (isStopping && a>0)
    {
        a -= dt*0.060606;
        if (a<0)
            a = 0;
        spr.setColor(Color(255,255,255,a));
    }
}

void FogWeather::draw(RenderTarget& window)
{
    for (unsigned int i = 0; i<particles.size(); ++i)
    {
        spr.setPosition(particles[i].x-owner->getCamera().x,particles[i].y-owner->getCamera().y);
        spr.setRotation(particles[i].rotation);
        window.draw(spr);
    }
}

SandstormWeather::SandstormWeather(Map* m) : BaseWeatherType(m), cover(Vector2f(800,600))
{
    isStopping = false;
    lastTime = Weather::timer.getElapsedTime().asMilliseconds();
    a = 0;
    lChng = 30;
    mainTxtr = imagePool.loadResource(Properties::MiscImagePath+"sandMain.png");
    swirlTxtr = imagePool.loadResource(Properties::MiscImagePath+"sandSwirl.png");
    mainSpr.setTexture(*mainTxtr);
    mainSpr.setOrigin(Vector2f(mainTxtr->getSize().x/2,mainTxtr->getSize().y/2));
    swirlSpr.setTexture(*swirlTxtr);
    cover.setFillColor(Color(230,230,90,0));
    mainSpr.setColor(Color(255,255,255,0));
    swirlSpr.setColor(Color(255,255,255,0));

    int minX = owner->getCamera().x-100;
    int minY = owner->getCamera().y-100;
    for (int x = 0; x<16; x++)
    {
        for (int y = 0; y<14; y++)
        {
            sand.push_back(Vector2f(minX+x*98,minY+y*55));
        }
    }
    for (int i = 0; i<40; ++i)
    {
        swirls.push_back(Vector3f(Random(minX+200,minX+1468),Random(minY+200,minY+900),Random(0,360)));
    }
}

SandstormWeather::~SandstormWeather()
{
    //
}

void SandstormWeather::stop()
{
    isStopping = true;
}

int SandstormWeather::getLightChange()
{
    return lChng;
}

bool SandstormWeather::isFinished()
{
    return mainSpr.getColor().a==0;
}

void SandstormWeather::update()
{
    int minX = owner->getCamera().x - 100;
    int minY = owner->getCamera().y - 100;
    int sX = owner->getCamera().x + 1468;
    int sY = owner->getCamera().y + 660;
    double dt = Weather::timer.getElapsedTime().asMilliseconds()-lastTime;
    lastTime = Weather::timer.getElapsedTime().asMilliseconds();
    double dx1 = dt*0.363636, dy1 = dt*0.090909;
    double dx2 = dt*0.545454, dy2 = dt*0.121212, dz = dt*0.454545;

    for (unsigned int i = 0; i<sand.size(); ++i)
    {
        sand[i].x -= dx1;
        sand[i].y -= dy1;
        if (sand[i].x<minX)
            sand[i].x = sX;
        if (sand[i].y<minY)
            sand[i].y = sY;
    }
    for (unsigned int i = 0; i<swirls.size(); ++i)
    {
        swirls[i].x -= dx2;
        swirls[i].y -= dy2;
        swirls[i].z -= dz;
        if (swirls[i].x<minX || swirls[i].y<minY)
            swirls[i] = Vector3f(sX,Random(minY+200,minY+900),Random(0,360));
    }

    if (isStopping)
    {
        a -= dt*0.151515;
        if (a<0)
            a = 0;
        mainSpr.setColor(Color(255,255,255,a));
        swirlSpr.setColor(Color(255,255,255,a));
        a -= 5.303;
        if (a<0)
            a = 0;
        cover.setFillColor(Color(230,230,90,a));
    }
    else if (mainSpr.getColor().a<255)
    {
        a += dt*0.151515;
        if (a>255)
            a = 255;
        mainSpr.setColor(Color(255,255,255,a));
        swirlSpr.setColor(Color(255,255,255,a));
        if (a>2.4242)
            a = 80;
        cover.setFillColor(Color(230,230,90,a));
    }
}

void SandstormWeather::draw(RenderTarget& window)
{
    window.draw(cover);
    for (unsigned int i = 0; i<sand.size(); ++i)
    {
        mainSpr.setPosition(Vector2f(sand[i]-owner->getCamera()));
        mainSpr.setRotation(sin((sand[i].x+sand[i].y)/160*3.1415926)*30);
        window.draw(mainSpr);
    }
    for (unsigned int i = 0; i<swirls.size(); ++i)
    {
        swirlSpr.setPosition(Vector2f(swirls[i].x,swirls[i].y)-owner->getCamera());
        swirlSpr.setRotation(swirls[i].z);
        window.draw(swirlSpr);
    }
}

Weather::Weather(Map* m)
{
    owner = m;
    weather = nullptr;
    nextChange = 0;
    type = None;
    isStopping = false;
    keepRemembered = false;
    curLight = 0;
    desiredLight = 0;
}

void Weather::init(Type tp, bool force)
{
	cout << "Set weather: " << tp << endl;

    if (weather && !keepRemembered)
    {
    	if (tp!=None || force)
        {
        	weather.reset();
			weather = nullptr;
        }
        else
			weather->stop();
    }

    type = tp;
    if (type==LightRain)
        weather.reset(new RainWeather(owner,sEngine,false,false));
    else if (type==LightRainThunder)
        weather.reset(new RainWeather(owner,sEngine,false,true));
    else if (type==HardRain)
        weather.reset(new RainWeather(owner,sEngine,true,false));
    else if (type==HardRainThunder)
        weather.reset(new RainWeather(owner,sEngine,true,true));
    else if (type==LightSnow)
        weather.reset(new SnowWeather(owner,sEngine,false,false));
    else if (type==LightSnowThunder)
        weather.reset(new SnowWeather(owner,sEngine,false,true));
    else if (type==HardSnow)
        weather.reset(new SnowWeather(owner,sEngine,true,false));
    else if (type==HardSnowThunder)
        weather.reset(new SnowWeather(owner,sEngine,true,true));
    else if (type==Sunny)
        weather.reset(new SunnyWeather());
    else if (type==ThinFog)
        weather.reset(new FogWeather(owner,false));
    else if (type==ThickFog)
        weather.reset(new FogWeather(owner,true));
    else if (type==SandStorm)
        weather.reset(new SandstormWeather(owner));

	if (type!=AllRandom && type!=WaterRandom && type!=SnowRandom && type!=DesertRandom && type!=None)
		logWeather(type);

	nextChange = Weather::timer.getElapsedTime().asMilliseconds()+Random(180000,420000);
}

bool Weather::enterMap(string name)
{
	curMap = name;
	bool ret = false;

	if (pastWeather.find(name)!=pastWeather.end())
	{
		long age = Weather::timer.getElapsedTime().asMilliseconds()-pastWeather[name].timeRecorded;
		if (age<300000) //5 minutes
		{
			init(pastWeather[name].type);
			ret = true;
			keepRemembered = true;
		}
		pastWeather.erase(name);
	}

	return ret;
}

void Weather::logWeather(Type tp)
{
	PastWeather t;
	t.mapName = curMap;
	t.timeRecorded = Weather::timer.getElapsedTime().asMilliseconds();
	t.type = tp;
	pastWeather[curMap] = t;
}

void Weather::setOwner(Map* o) {
	owner = o;
	if (weather) {
		weather->setOwner(o);
	}
}

void Weather::update()
{
    if (curLight<desiredLight)
        curLight += 1;
    if (curLight>desiredLight)
        curLight -= 1;

    desiredLight = 0;
    if (weather)
    {
        weather->update();
        desiredLight = weather->getLightChange();
        if (Weather::timer.getElapsedTime().asMilliseconds()>=nextChange && (type==WaterRandom || type==SnowRandom || type==DesertRandom || type==AllRandom) && !isStopping)
        {
            weather->stop();
            isStopping = true;
        }
        if (isStopping)
        {
            if (weather->isFinished())
            {
                isStopping = false;
                weather.reset();
                weather = nullptr;
                nextChange = Weather::timer.getElapsedTime().asMilliseconds()+Random(180000,420000);
            }
        }
    }

    else if (Weather::timer.getElapsedTime().asMilliseconds()>=nextChange)
    {
        if (type==WaterRandom)
            createRain();
        else if (type==SnowRandom)
            createSnow();
        else if (type==AllRandom)
        {
            int w = Random(0,500);
            if (w<100)
                createRain();
            else if (w<200)
                createSnow();
            else if (w<300)
			{
				weather.reset(new SunnyWeather());
				logWeather(Sunny);
			}
            else if (w<400)
			{
				bool isThick = Random(0,1000)<500;
				weather.reset(new FogWeather(owner,isThick));
				logWeather(isThick?(ThickFog):(ThinFog));
			}
            else
			{
				weather.reset(new SandstormWeather(owner));
				logWeather(SandStorm);
			}
        }
        nextChange = Weather::timer.getElapsedTime().asMilliseconds()+Random(120000,600000);
    }
}

void Weather::createRain()
{
    bool isHard = Random(0,1000)<500;
    bool canThunder = Random(0,1000)<500;
    if (isHard && canThunder)
		logWeather(HardRainThunder);
	else if (isHard && !canThunder)
		logWeather(HardRain);
	if (!isHard && canThunder)
		logWeather(LightRainThunder);
	else if (!isHard && !canThunder)
		logWeather(LightRain);
    weather.reset(new RainWeather(owner,sEngine,isHard,canThunder));
}

void Weather::createSnow()
{
    bool isHard = Random(0,1000)<500;
    bool canThunder = Random(0,1000)<500;
	if (isHard && canThunder)
		logWeather(HardSnowThunder);
	else if (isHard && !canThunder)
		logWeather(HardSnow);
	if (!isHard && canThunder)
		logWeather(LightSnowThunder);
	else if (!isHard && !canThunder)
		logWeather(LightSnow);
    weather.reset(new SnowWeather(owner,sEngine,isHard,canThunder));
}

void Weather::draw(RenderTarget& window)
{
    if (weather)
        weather->draw(window);
}

int Weather::getLightModifier()
{
    return curLight;
}
