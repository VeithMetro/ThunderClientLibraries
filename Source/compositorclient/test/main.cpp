/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 Metrological B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MODULE_NAME
#define MODULE_NAME CompositorClientTest
#endif

#include <core/core.h>
#include <localtracer/localtracer.h>
#include <messaging/messaging.h>
#include <plugins/plugins.h>

#include <interfaces/IComposition.h>
#include <interfaces/ICompositionBuffer.h>

#include <compositor/Client.h>

MODULE_NAME_DECLARATION(BUILD_REFERENCE)

namespace WPEFramework {

} // namespace WPEFramework

int main(int argc, const char* argv[])
{
    WPEFramework::Messaging::LocalTracer& tracer = WPEFramework::Messaging::LocalTracer::Open();

    const char* executableName(WPEFramework::Core::FileNameOnly(argv[0]));

    {
        WPEFramework::Messaging::ConsolePrinter printer(true);

        tracer.Callback(&printer);

        const std::vector<string> modules = {
            "CompositorClientTest",
            "Composition_Client"
        };

        for (auto module : modules) {
            tracer.EnableMessage(module, "", true);
        }

        TRACE_GLOBAL(WPEFramework::Trace::Information, ("%s - build: %s", executableName, __TIMESTAMP__));

        WPEFramework::Compositor::IDisplay* display = WPEFramework::Compositor::IDisplay::Instance("HDMI-1");
        WPEFramework::Compositor::IDisplay::ISurface* surface = nullptr;

        char keyPress;

        do {
            keyPress = toupper(getchar());

            switch (keyPress) {
            case 'C': {
                ASSERT(display != nullptr);

                if (surface != nullptr) {
                    surface = display->Create("TestClient", 1280, 720);
                    TRACE_GLOBAL(WPEFramework::Trace::Information, ("Create %s surface", surface->Name().c_str()));
                } else {
                    TRACE_GLOBAL(WPEFramework::Trace::Information, ("Release %s surface", surface->Name().c_str()));
                    surface->Release();
                    surface = nullptr;
                }

                break;
            }
            case 'I': {
                if(surface != nullptr) {
                    TRACE_GLOBAL(WPEFramework::Trace::Information, ("Surface Info\n Name: %s\n Width: %d\n Height: %d\n",   
                        surface->Name().c_str(), 
                        surface->Width(), 
                        surface->Height())); 
                }
            }
            default:
                break;
            }
        } while (keyPress != 'Q');

        TRACE_GLOBAL(WPEFramework::Trace::Information, ("Exiting %s.... ", executableName));

        if (surface != nullptr) {
            surface->Release();
        }

        if (display != nullptr) {
            display->Release();
        }
    }

    tracer.Close();
    WPEFramework::Core::Singleton::Dispose();

    return 0;
}
