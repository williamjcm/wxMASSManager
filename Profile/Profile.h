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
        auto setStoryProgress(std::int32_t progress) -> bool;

        auto lastMissionId() const -> std::int32_t;
        auto getLastMissionId() -> std::int32_t;

        auto verseSteel() const -> std::int32_t;
        auto getVerseSteel() -> std::int32_t;
        auto setVerseSteel(std::int32_t amount) -> bool;

        auto undinium() const -> std::int32_t;
        auto getUndinium() -> std::int32_t;
        auto setUndinium(std::int32_t amount) -> bool;

        auto necriumAlloy() const -> std::int32_t;
        auto getNecriumAlloy() -> std::int32_t;
        auto setNecriumAlloy(std::int32_t amount) -> bool;

        auto lunarite() const -> std::int32_t;
        auto getLunarite() -> std::int32_t;
        auto setLunarite(std::int32_t amount) -> bool;

        auto asterite() const -> std::int32_t;
        auto getAsterite() -> std::int32_t;
        auto setAsterite(std::int32_t amount) -> bool;

        auto ednil() const -> std::int32_t;
        auto getEdnil() -> std::int32_t;
        auto setEdnil(std::int32_t amount) -> bool;

        auto nuflalt() const -> std::int32_t;
        auto getNuflalt() -> std::int32_t;
        auto setNuflalt(std::int32_t amount) -> bool;

        auto aurelene() const -> std::int32_t;
        auto getAurelene() -> std::int32_t;
        auto setAurelene(std::int32_t amount) -> bool;

        auto soldus() const -> std::int32_t;
        auto getSoldus() -> std::int32_t;
        auto setSoldus(std::int32_t amount) -> bool;

        auto synthesizedN() const -> std::int32_t;
        auto getSynthesizedN() -> std::int32_t;
        auto setSynthesizedN(std::int32_t amount) -> bool;

        auto alcarbonite() const -> std::int32_t;
        auto getAlcarbonite() -> std::int32_t;
        auto setAlcarbonite(std::int32_t amount) -> bool;

        auto keriphene() const -> std::int32_t;
        auto getKeriphene() -> std::int32_t;
        auto setKeriphene(std::int32_t amount) -> bool;

        auto nitinolCM() const -> std::int32_t;
        auto getNitinolCM() -> std::int32_t;
        auto setNitinolCM(std::int32_t amount) -> bool;

        auto quarkium() const -> std::int32_t;
        auto getQuarkium() -> std::int32_t;
        auto setQuarkium(std::int32_t amount) -> bool;

        auto alterene() const -> std::int32_t;
        auto getAlterene() -> std::int32_t;
        auto setAlterene(std::int32_t amount) -> bool;

        auto mixedComposition() const -> std::int32_t;
        auto getMixedComposition() -> std::int32_t;
        auto setMixedComposition(std::int32_t amount) -> bool;

        auto voidResidue() const -> std::int32_t;
        auto getVoidResidue() -> std::int32_t;
        auto setVoidResidue(std::int32_t amount) -> bool;

        auto muscularConstruction() const -> std::int32_t;
        auto getMuscularConstruction() -> std::int32_t;
        auto setMuscularConstruction(std::int32_t amount) -> bool;

        auto mineralExoskeletology() const -> std::int32_t;
        auto getMineralExoskeletology() -> std::int32_t;
        auto setMineralExoskeletology(std::int32_t amount) -> bool;

        auto carbonizedSkin() const -> std::int32_t;
        auto getCarbonizedSkin() -> std::int32_t;
        auto setCarbonizedSkin(std::int32_t amount) -> bool;

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

        std::int32_t _verseSteel;
        std::int32_t _undinium;
        std::int32_t _necriumAlloy;
        std::int32_t _lunarite;
        std::int32_t _asterite;
        std::int32_t _ednil;
        std::int32_t _nuflalt;
        std::int32_t _aurelene;
        std::int32_t _soldus;
        std::int32_t _synthesizedN;
        std::int32_t _alcarbonite;
        std::int32_t _keriphene;
        std::int32_t _nitinolCM;
        std::int32_t _quarkium;
        std::int32_t _alterene;

        std::int32_t _mixedComposition;
        std::int32_t _voidResidue;
        std::int32_t _muscularConstruction;
        std::int32_t _mineralExoskeletology;
        std::int32_t _carbonizedSkin;
};

#endif //PROFILE_H
