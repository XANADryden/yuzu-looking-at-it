// Copyright 2018 yuzu emulator team
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <cinttypes>
#include <memory>
#include "common/common_types.h"
#include "common/logging/log.h"
#include "common/string_util.h"
#include "core/file_sys/disk_filesystem.h"
#include "core/file_sys/savedata_factory.h"
#include "core/hle/kernel/process.h"

namespace FileSys {

SaveData_Factory::SaveData_Factory(std::string nand_directory)
    : nand_directory(std::move(nand_directory)) {}

ResultVal<std::unique_ptr<FileSystemBackend>> SaveData_Factory::Open(const Path& path) {
    std::string save_directory = GetFullPath();
    // Return an error if the save data doesn't actually exist.
    if (!FileUtil::IsDirectory(save_directory)) {
        // TODO(Subv): Find out correct error code.
        return ResultCode(-1);
    }

    auto archive = std::make_unique<Disk_FileSystem>(save_directory);
    return MakeResult<std::unique_ptr<FileSystemBackend>>(std::move(archive));
}

ResultCode SaveData_Factory::Format(const Path& path) {
    LOG_WARNING(Service_FS, "Format archive %s", GetName().c_str());
    // Create the save data directory.
    if (!FileUtil::CreateFullPath(GetFullPath())) {
        // TODO(Subv): Find the correct error code.
        return ResultCode(-1);
    }

    return RESULT_SUCCESS;
}

ResultVal<ArchiveFormatInfo> SaveData_Factory::GetFormatInfo(const Path& path) const {
    LOG_ERROR(Service_FS, "Unimplemented GetFormatInfo archive %s", GetName().c_str());
    // TODO(bunnei): Find the right error code for this
    return ResultCode(-1);
}

std::string SaveData_Factory::GetFullPath() const {
    u64 title_id = Kernel::g_current_process->program_id;
    // TODO(Subv): Somehow obtain this value.
    u32 user = 0;
    return Common::StringFromFormat("%ssave/%016" PRIX64 "/%08X/", nand_directory.c_str(), title_id,
                                    user);
}

} // namespace FileSys