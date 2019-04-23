#include "Shared/Scripts/ScriptManager.hpp"
#include "Shared/Util/File.hpp"
using namespace std;
using namespace sf;

namespace {
    void scriptRunner(shared_ptr<ScriptManager::ScriptData> d) {
        d->script->run();
        d->finished = true;
    }
}

ScriptManager::ScriptManager() : thread(&ScriptManager::update,this)
{
    stopped = stopping = false;
    thread.launch();
}

ScriptManager::~ScriptManager()
{
    stopAll();
    stop();
}

ScriptManager* ScriptManager::get() {
    static ScriptManager manager;
    return &manager;
}

void ScriptManager::runScript(ScriptReference scr, bool concurrent)
{
	#ifdef GAME
	if (scr->isRunning())
		return;

	if (concurrent)
    {
    	shared_ptr<ScriptData> temp(new ScriptData());
		temp->script = scr;
		temp->thread.reset(new Thread(&scriptRunner,temp));
		temp->thread->launch();
		lock.lock();
		runningScripts.push_back(temp);
		lock.unlock();
    }
    else
		scr->run();
	#endif
}

void ScriptManager::stopAll()
{
    stop();
    lock.lock();
    for (auto i = runningScripts.begin(); i!=runningScripts.end(); ++i)
    {
        (*i)->script->stop();
        (*i)->thread->wait();
    }
    runningScripts.clear();
    stopped = stopping = false;
    thread.launch();
    lock.unlock();
}

void ScriptManager::update()
{
    while (!stopping)
    {
    	lock.lock();
        for (auto i = runningScripts.begin(); i!=runningScripts.end(); ++i)
        {
            if ((*i)->finished)
            {
                runningScripts.erase(i);
                break;
            }
        }
        lock.unlock();
        sleep(milliseconds(1000));
    }
    stopped = true;
}

void ScriptManager::save(File* file)
{
    file->write<uint16_t>(intSaveEntries.size());
    for (auto i = intSaveEntries.begin(); i!=intSaveEntries.end(); ++i)
    {
        file->writeString(i->first);
        file->write<int32_t>(i->second);
    }
    file->write<uint16_t>(stringSaveEntries.size());
    for (auto i = stringSaveEntries.begin(); i!=stringSaveEntries.end(); ++i)
    {
        file->writeString(i->first);
        file->writeString(i->second);
    }
}

void ScriptManager::load(File* file)
{
    intSaveEntries.clear();
    stringSaveEntries.clear();

    int sz = file->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
    {
        string t = file->getString();
        intSaveEntries[t] = file->get<int32_t>();
    }
    sz = file->get<uint16_t>();
    for (int i = 0; i<sz; ++i)
    {
        string t = file->getString();
        stringSaveEntries[t] = file->getString();
    }
}

void ScriptManager::stop()
{
	stopping = true;
	while (!stopped)
		sleep(milliseconds(300));
}
