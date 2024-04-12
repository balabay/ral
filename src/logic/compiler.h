#pragma once

#include <memory>
#include <string>
#include <vector>

namespace RaLang {

class Compiler {

public:
    Compiler();
    ~Compiler();
    void compile();
    void addSource(const char* fileName);
    void setEmitDebugInfo(bool enable);

private:
    bool m_emitDebugInfo;                         /* Generate debug information */
    std::vector<std::string> m_sources;             /* List of source filenames to compile */
};

} // namespace RaLang
