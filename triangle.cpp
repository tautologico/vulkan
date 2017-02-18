//
// Creating a Vulkan instance
//

#include <iostream>
#include <vulkan/vulkan.h>

using namespace std;

VkInstance inst;

void initInstance() {
    // Application info
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "instance";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "Huevos";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Vulkan instance
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;

    VkResult res;

    res = vkCreateInstance(&inst_info, NULL, &inst);

    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        cout << "Could not find a compatible Vulkan ICD" << endl;
        exit(-1);
    } else if (res) {
        cout << "Unknown error: " << res << endl;
        exit(-1);
    }
}

int main() {
    initInstance();

    cout << "Vulkan instance created successfully" << endl;

    VkResult res;
    uint32_t dev_count = 0;
    res = vkEnumeratePhysicalDevices(inst, &dev_count, NULL);
    if (res) {
        cout << "could not determine the number of devices" << endl;
        vkDestroyInstance(inst, NULL);
        exit(-1);
    }

    cout << "Found " << dev_count << " devices" << endl;

    vkDestroyInstance(inst, NULL);

    return 0;
}

