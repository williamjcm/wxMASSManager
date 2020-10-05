#ifndef PROFILE_H
#define PROFILE_H

#include <cstdint>

#include <string>

enum class ProfileType : std::uint8_t {
    Demo,
    FullGame
};

class Profile {
    public:
        explicit Profile(const std::string& path);

        auto valid() const -> bool;

        auto lastError() const -> std::string const&;

        auto filename() const -> std::string const&;

        auto type() const -> ProfileType;

        auto steamId() const -> std::string const&;

        auto companyName() const -> std::string const&;
        auto getCompanyName() -> std::string const&;
        auto renameCompany(const std::string& new_name) -> bool;

        auto activeFrameSlot() const -> std::int8_t;
        auto getActiveFrameSlot() -> std::int8_t;

        auto credits() const -> std::int32_t;
        auto getCredits() -> std::int32_t;

        auto storyProgress() const -> std::int32_t;
        auto getStoryProgress() -> std::int32_t;

        auto lastMissionId() const -> std::int32_t;
        auto getLastMissionId() -> std::int32_t;

        auto backup(const std::string& filename) -> bool;

    private:
        std::string _profileDirectory;
        std::string _filename;

        ProfileType _type;

        std::string _steamId;

        bool        _valid = false;
        std::string _lastError = "";

        std::string _companyName;

        std::int8_t _activeFrameSlot = 0;

        std::int32_t _credits;

        std::int32_t _storyProgress;

        std::int32_t _lastMissionId;
};

#endif //PROFILE_H
