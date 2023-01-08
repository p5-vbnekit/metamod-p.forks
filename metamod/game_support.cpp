// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// game_support.cpp - info to recognize different HL mod "games"

/*
 * Copyright (c) 2001-2013 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include <cstddef>
#include <cstring>

#include <string>

#include <fcntl.h>              // open, write

#include <extdll.h>             // always

#include "game_support.h"       // me
#include "log_meta.h"           // META_LOG, etc
#include "types_meta.h"         // mBOOL
#include "osdep.h"              // win32 snprintf, etc
#include "game_autodetect.h"    // autodetect_gamedll
#include "support_meta.h"       // MIN


// Adapted from adminmod h_export.cpp:
//! this structure contains a list of supported mods and their dlls names
//! To add support for another mod add an entry here, and add all the
//! exported entities to link_func.cpp
game_modlist_t const known_games = {
    // name/gamedir linux_so win_dll desc
    //
    // Previously enumerated in this sourcefile, the list is now kept in a
    // separate file, generated based on game information stored in a
    // convenient db.
    //
#include "games.h"
    // End of list terminator:
    {NULL, NULL, NULL, NULL}
};

// Find a modinfo corresponding to the given game name.
game_modinfo_t const * DLLINTERNAL lookup_game(char const *name) {
    for (int i = 0; known_games[i].name; i++) {
        game_modinfo_t const * const imod = &known_games[i];
        // If there are 2 or more same names check next dll file if doesn't exist
        if (strcasematch(imod->name, name)) {
            char check_path[NAME_MAX];
            safevoid_snprintf(
                check_path, sizeof(check_path), "dlls/%s",
#ifdef _WIN32
                imod->win_dll
#elif defined(linux)
                imod->linux_so
#endif
            );
            if (! valid_gamedir_file(check_path)) continue;
            return imod;
        }
    }

    // no match found
    return NULL;
}

// Installs gamedll from Steam cache
mBOOL DLLINTERNAL install_gamedll(char *from, char const *to) {
    if (! from) return mFALSE;

    int length_in;
    byte * const cachefile = LOAD_FILE_FOR_ME(from, &length_in);

    if (! cachefile) {
        META_DEBUG(3, ("Failed to install gamedll from cache: file %s not found in cache.", from));
        return mFALSE;
    }

    // If the file seems to exist in the cache.
    if (! to) to = from;
    int fd = ::open(to, O_WRONLY|O_CREAT|O_EXCL|O_BINARY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);

    if (fd < 0) {
        META_DEBUG(3, ("Installing gamedll from cache: Failed to create file %s: %s", to, ::std::strerror(errno)));
        FREE_FILE(cachefile);
        return mFALSE;
    }

    int const length_out = ::write(fd, cachefile, length_in);
    FREE_FILE(cachefile);
    ::close(fd);

    // Writing the file was not successfull
    if (length_out != length_in) {
        META_DEBUG(3, (
            "Installing gamedll from chache: Failed to write all %d bytes to file, only %d written: %s",
            length_in, length_out, ::std::strerror(errno))
        );

        // Let's not leave a mess but clean up nicely.
        if (length_out >= 0) unlink(to);

        return mFALSE;
    }

    META_LOG("Installed gamedll %s from cache.", to);
    return mTRUE;
}

// Set all the fields in the gamedll struct, - based either on an entry in
// known_games matching the current gamedir, or on one specified manually
// by the server admin.
//
// meta_errno values:
//  - ME_NOTFOUND   couldn't recognize game
mBOOL DLLINTERNAL setup_gamedll(gamedll_t *gamedll) {
    if (true) {
        static bool flag_= false;
        if (flag_) {
            META_ERROR("Logic bug: setup_gamedll is already called!");
            meta_errno = ME_NOTALLOWED;
            return(mFALSE);
        }
        flag_ = true;
    }

    // Check for old-style "metagame.ini" file and complain.
    if (valid_gamedir_file(OLD_GAMEDLL_TXT)) META_WARNING(
        "File '%s' is no longer supported; instead, specify override gamedll in %s or with '+localinfo mm_gamedll <dllfile>'",
        OLD_GAMEDLL_TXT, CONFIG_INI
    );

    char const *knownfn = 0;

    // First, look for a known game, based on gamedir.
    game_modinfo_t const * const known = lookup_game(gamedll->name);
    if (known) {
#ifdef _WIN32
        knownfn = known->win_dll;
#elif defined(linux)
        knownfn = known->linux_so;
    #ifdef __x86_64__
        // AMD64: convert _i386.so to _amd64.so
        if (true) {
            struct Helper_ {
                inline static char const * routine(char const *src) {
                    if (! src) return src;
                    ::std::string name_ = src;
                    ::std::size_t const name_size_ = name_.size();
                    static ::std::size_t const pattern_size_ = sizeof("_i386.so") - 1;
                    if (! (name_size_ > pattern_size_)) return src;
                    if (true) {
                        char &char_ = name_.at(name_size_ - sizeof("86.so"));
                        if ('3' != char_) {
                            if (('3' < char_) && ('7' > char_)) char_ = '3';
                            else return src;
                        }
                    }
                    ::std::size_t const offset_ = name_size_ - pattern_size_;
                    if (0 != ::std::strcmp("_i386.so", name_.data() + offset_)) return src;
                    name_.resize(offset_);
                    // pointer is given outside function
                    static ::std::string const fixed_ = name_ + "_amd64.so";
                    return fixed_.data();
                }
            };
            knownfn = Helper_::routine(knownfn);
        }
    #endif /*__x86_64__*/
#else
#error "OS unrecognized"
#endif /* _WIN32 */

        // Do this before autodetecting gamedll from "dlls/*.dll"
        if (! Config->gamedll) {
            char const *usedfn = 0;

#ifdef linux
            // The engine changed game dll lookup behaviour in that it strips
            // anything after the last '_' from the name and tries to load the
            // resulting name. The DSO names were changed and do not have the
            // '_i386' part in them anymore, so cs_i386.so became cs.so. We
            // have to adapt to that and try to load the DSO name without the
            // '_*' part first, to see if we have a new version file available.
            char temp_str[NAME_MAX];
            STRNCPY(temp_str, knownfn, sizeof(temp_str));
            char const * const strippedfn = temp_str;
            char * const loc = ::std::strrchr(temp_str, '_');

            // A small safety net here: make sure that we are dealing with
            // a file name at least four characters long and ending in
            // '.so'. This way we can be sure that we can safely overwrite
            // anything from the '_' on with '.so'.
            char const *ext;
            ::std::size_t size = 0;
            if (0 != loc) {
                size = ::std::strlen(strippedfn);
                ext = strippedfn + (size - 3);
            }

            if ((0 != loc) && (3 < size) && (0 == strcasecmp(ext, ".so"))) {
                ::std::strcpy(loc, ".so");
                META_DEBUG(4, ("Checking for new version game DLL name '%s'.\n", strippedfn));

                // Again, as above, I abuse the real_pathname member to store the full pathname
                // and the pathname member to store the relative name to pass it to the
                // install_gamedll function to save stack space. They are going
                // to get overwritten later on, so that's ok.
                safevoid_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "dlls/%s", strippedfn);
                // Check if the gamedll file exists. If not, try to install it from
                // the cache.
                bool ok = true;
                if (! valid_gamedir_file(gamedll->pathname)) {
                    safevoid_snprintf(
                        gamedll->real_pathname, sizeof(gamedll->real_pathname),
                        "%s/dlls/%s", gamedll->gamedir, strippedfn
                    );
                    ok = install_gamedll(gamedll->pathname, gamedll->real_pathname);
                }
                if (ok) usedfn = strippedfn;
            }
            else META_DEBUG(4, (
                "Known game DLL name does not qualify for checking for a stripped version, skipping: '%s'.\n",
                strippedfn
            ));
#endif /* linux */
            if (usedfn) knownfn = usedfn;
            else {
                // If no file to be used was found, try the old known DLL file name.
                META_DEBUG(4, ("Checking for old version game DLL name '%s'.\n", knownfn));
                safevoid_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "dlls/%s", knownfn);
                // Check if the gamedll file exists. If not, try to install it from
                // the cache.
                if (! valid_gamedir_file(gamedll->pathname)) {
                    safevoid_snprintf(
                        gamedll->real_pathname, sizeof(gamedll->real_pathname),
                        "%s/dlls/%s", gamedll->gamedir, knownfn
                    );
                    install_gamedll(gamedll->pathname, gamedll->real_pathname);
                }
            }
        }
    }

    char const *autofn = 0;

    // Then, autodetect gamedlls in "gamedir/dlls/"
    // autodetect_gamedll returns 0 if knownfn exists and is valid gamedll.
    if (Config->autodetect && (autofn = autodetect_gamedll(gamedll, knownfn))) {
        // If knownfn is set and autodetect_gamedll returns non-null
        // then knownfn doesn't exists and we should use autodetected
        // dll instead.
        if (knownfn) {
            // Whine loud about fact that known-list dll doesn't exists!
            // META_LOG(plapla);
            knownfn = autofn;
        }
    }

    // Neither override nor known-list nor auto-detect found a gamedll.
    if ((! known) && (! Config->gamedll) && (! autofn)) RETURN_ERRNO(mFALSE, ME_NOTFOUND);

    bool override = false;

    // Use override-dll if specified.
    if (Config->gamedll) {
        STRNCPY(gamedll->pathname, Config->gamedll, sizeof(gamedll->pathname));
        override = true;

        // If the path is relative, the gamedll file will be missing and
        // it might be found in the cache file.
        if (! is_absolute_path(gamedll->pathname)) {
            char install_path[NAME_MAX];
            safevoid_snprintf(
                install_path, sizeof(install_path),
                "%s/%s", gamedll->gamedir, gamedll->pathname
            );
            // If we could successfully install the gamedll from the cache we
            // rectify the pathname to be a full pathname.
            if (install_gamedll(gamedll->pathname, install_path)) STRNCPY(
                gamedll->pathname, install_path, sizeof(gamedll->pathname)
            );
        }
    }
    // Else use Known-list dll.
    else if (known) safevoid_snprintf(
        gamedll->pathname, sizeof(gamedll->pathname),
        "%s/dlls/%s", gamedll->gamedir, knownfn
    );
    // Else use Autodetect dll.
    else safevoid_snprintf(
        gamedll->pathname, sizeof(gamedll->pathname),
        "%s/dlls/%s", gamedll->gamedir, autofn
    );

    if (true) {
        // get filename from pathname
        char const * const cp = ::std::strrchr(gamedll->pathname, '/');
        gamedll->file = cp ? (1 + cp) : gamedll->pathname;
    }

    // If found, store also the supposed "real" dll path based on the
    // gamedir, in case it differs from the "override" dll path.
    if (known && override) safevoid_snprintf(
        gamedll->real_pathname, sizeof(gamedll->real_pathname),
        "%s/dlls/%s", gamedll->gamedir, knownfn
    );
    else if (known && autofn) safevoid_snprintf(
        gamedll->real_pathname, sizeof(gamedll->real_pathname),
        "%s/dlls/%s", gamedll->gamedir, knownfn
    );
    else STRNCPY( // (! known) or ((! override) and (! autofn))
        gamedll->real_pathname, gamedll->pathname,
        sizeof(gamedll->real_pathname)
    );

    static char autodetect_desc_buf[NAME_MAX]; // pointer is given outside function

    if (override) {
        // generate a desc
        static char override_desc_buf[NAME_MAX]; // pointer is given outside function
        safevoid_snprintf(override_desc_buf, sizeof(override_desc_buf), "%s (override)", gamedll->file);
        gamedll->desc = override_desc_buf;
        // log result
        META_LOG("Overriding game '%s' with dllfile '%s'", gamedll->name, gamedll->file);
    }
    else if (known && autofn) {
        // dll in known-list doesn't exists but we found new one with autodetect.
        // generate a desc
        safevoid_snprintf(autodetect_desc_buf, sizeof(autodetect_desc_buf), "%s (autodetect-override)", gamedll->file);
        gamedll->desc = autodetect_desc_buf;
        META_LOG("Recognized game '%s'; Autodetection override; using dllfile '%s'", gamedll->name, gamedll->file);
    }
    else if (autofn) {
        // generate a desc
        safevoid_snprintf(autodetect_desc_buf, sizeof(autodetect_desc_buf), "%s (autodetect)", gamedll->file);
        gamedll->desc = autodetect_desc_buf;
        META_LOG("Autodetected game '%s'; using dllfile '%s'", gamedll->name, gamedll->file);
    }
    else if (known) {
        gamedll->desc = known->desc;
        META_LOG("Recognized game '%s'; using dllfile '%s'", gamedll->name, gamedll->file);
    }

    return mTRUE;
}
