#include "Config.h"
#include "Utils.h"

using namespace FileUtils;
namespace Config
{
    bool ensureDirectories(){
        bool ok = true;

        ok &= ensureDir(DATA_ROOT);
        ok &= ensureDir(PACIENTES_DIR);
        ok &= ensureDir(RELATORIOS_DIR);
        ok &= ensureDir(SEGURANCA_DIR);
        return ok;
    }
}
