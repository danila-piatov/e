#pragma once

namespace e {

void InitSignalHandlers();

bool WasQuitSignalReceived();

bool WasWindowResizeSignalReceived();

} // namespace e