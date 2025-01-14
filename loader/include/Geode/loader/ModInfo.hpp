#pragma once

#include "Types.hpp"
#include "../external/json/json.hpp"
#include "../utils/VersionInfo.hpp"
#include "../utils/Result.hpp"

namespace geode {
    namespace utils::file {
        class Unzip;
    }

    using ModJson = nlohmann::ordered_json;

    struct Dependency {
        std::string m_id;
        // todo: Dynamic versions (1.*.*)
        VersionInfo m_version { 1, 0, 0 };
        ModResolveState m_state = ModResolveState::Unloaded;
        bool m_required = false;
        Mod* m_mod = nullptr;
        bool isUnresolved() const;
    };

    struct IssuesInfo {
        std::string m_info;
        std::optional<std::string> m_url;
    };

    /**
     * Represents all the data gatherable
     * from mod.json
     */
    struct GEODE_DLL ModInfo {
        /**
         * Path to the mod file
         */
        ghc::filesystem::path m_path;
        /**
         * Name of the platform binary within
         * the mod zip
         */
        std::string m_binaryName;
        /**
         * Mod Version. Should follow semver.
         */
        VersionInfo m_version { 1, 0, 0 };
        /**
         * Human-readable ID of the Mod.
         * Recommended to be in the format
         * "com.developer.mod". Not
         * guaranteed to be either case-
         * nor space-sensitive. Should
         * be restricted to the ASCII
         * character set.
         */
        std::string m_id;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        std::string m_name;
        /**
         * The name of the head developer.
         * Should be a single name, like
         * "HJfod" or "The Geode Team".
         * If the mod has multiple
         * developers, this field should
         * be one of their name or a team
         * name, and the rest of the credits
         * should be named in `m_credits`
         * instead.
         */
        std::string m_developer;
        /**
         * Short & concise description of the
         * mod.
         */
        std::optional<std::string> m_description;
        /**
         * Detailed description of the mod, writtenin Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        std::optional<std::string> m_details;
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        std::optional<std::string> m_changelog;
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see <Geode/ui/MDTextArea.hpp>) for more info
         */
        std::optional<std::string> m_supportInfo;
        /**
         * Git Repository of the mod
         */
        std::optional<std::string> m_repository;
        /**
         * Info about where users should report issues and request help
         */
        std::optional<IssuesInfo> m_issues;
        /**
         * Dependencies
         */
        std::vector<Dependency> m_dependencies;
        /**
         * Mod spritesheet names
         */
        std::vector<std::string> m_spritesheets;
        /**
         * Mod settings
         */
        std::vector<std::pair<std::string, std::shared_ptr<Setting>>> m_settings;
        /**
         * Whether the mod can be disabled or not
         */
        bool m_supportsDisabling = true;
        /**
         * Whether the mod can be unloaded or not
         */
        bool m_supportsUnloading = false;
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */
        bool m_needsEarlyLoad = false;
        /**
         * Create ModInfo from an unzipped .geode package
         */
        static Result<ModInfo> createFromGeodeZip(utils::file::Unzip& zip);
        /**
         * Create ModInfo from a .geode package
         */
        static Result<ModInfo> createFromGeodeFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        static Result<ModInfo> createFromFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a parsed json document
         */
        static Result<ModInfo> create(ModJson const& json);

        /**
         * Convert to JSON. Essentially same as getRawJSON except dynamically 
         * adds runtime fields like path
         */
        ModJson toJSON() const;
        /**
         * Get the raw JSON file
         */
        ModJson getRawJSON() const;

        bool operator==(ModInfo const& other) const;

        static bool validateID(std::string const& id);

    private:
        ModJson m_rawJSON;

        /**
         * Version is passed for backwards
         * compatibility if we update the mod.json
         * format
         */
        static Result<ModInfo> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    // For converting ModInfo back to JSON
    void GEODE_DLL to_json(nlohmann::json& json, ModInfo const& info);
}
