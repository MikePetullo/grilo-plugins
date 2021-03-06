/*
 * Copyright (C) 2016 Grilo Project
 *
 * Author: Victor Toso <me@victortoso.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include "test_lua_factory_utils.h"

#define ACOUSTID_ID  "grl-acoustid"
#define CHROMAPRINT_ID "grl-chromaprint"

#define TEST_PLUGINS_PATH  LUA_FACTORY_PLUGIN_PATH ";" CHROMAPRINT_PLUGIN_PATH
#define TEST_PLUGINS_LOAD  LUA_FACTORY_ID ":" CHROMAPRINT_ID

#define ACOUSTID_OPS GRL_OP_RESOLVE

#define GRESOURCE_PREFIX "resource:///org/gnome/grilo/plugins/test/acoustid/data/"

#define FINGERPRINT_LUDOVICO_EI  GRESOURCE_PREFIX "chromaprint_ludovico_einaudi_primavera.txt"
#define FINGERPRINT_NORAH_JONES  GRESOURCE_PREFIX "chromaprint_norah_jones_chasing_pirates.txt"
#define FINGERPRINT_PHILIP_GLAS  GRESOURCE_PREFIX "chromaprint_philip_glass_the_passion_of.txt"
#define FINGERPRINT_TROMBONE_SH  GRESOURCE_PREFIX "chromaprint_trombone_shorty_buckjump.txt"
#define FINGERPRINT_RADIOHEAD_PA GRESOURCE_PREFIX "chromaprint_radiohead_paranoid_android.txt"

static gchar *
resolve (GrlSource   *source,
         const gchar *fingerprint,
         gint         duration,
         gchar      **out_mb_artist_id,
         gchar      **out_artist,
         gchar      **out_mb_album_id,
         gchar      **out_album,
         gchar      **out_mb_recording_id,
         gchar      **out_title)
{
  GList *keys;
  GrlMedia *audio;
  GrlOperationOptions *options;
  GrlRegistry *registry;
  GrlKeyID chromaprint_key;
  GError *error = NULL;

  registry = grl_registry_get_default ();
  chromaprint_key = grl_registry_lookup_metadata_key (registry, "chromaprint");
  g_assert_cmpint (chromaprint_key, !=, GRL_METADATA_KEY_INVALID);

  audio = grl_media_audio_new ();
  grl_data_set_string (GRL_DATA (audio), chromaprint_key, fingerprint);
  grl_media_set_duration (audio, duration);

  keys = grl_metadata_key_list_new (GRL_METADATA_KEY_MB_ARTIST_ID,
                                    GRL_METADATA_KEY_ARTIST,
                                    GRL_METADATA_KEY_MB_ALBUM_ID,
                                    GRL_METADATA_KEY_ALBUM,
                                    GRL_METADATA_KEY_MB_RECORDING_ID,
                                    GRL_METADATA_KEY_TITLE,
                                    NULL);
  options = grl_operation_options_new (NULL);
  grl_operation_options_set_resolution_flags (options, GRL_RESOLVE_NORMAL);

  grl_source_resolve_sync (source,
                           GRL_MEDIA (audio),
                           keys,
                           options,
                           &error);
  g_assert_no_error (error);

  *out_mb_artist_id = g_strdup (grl_media_get_mb_artist_id (audio));
  *out_artist = g_strdup (grl_media_get_artist (audio));
  *out_mb_album_id = g_strdup (grl_media_get_mb_album_id (audio));
  *out_album = g_strdup (grl_media_get_album (audio));
  *out_mb_recording_id = g_strdup (grl_media_get_mb_recording_id (audio));
  *out_title = g_strdup (grl_media_get_title (audio));

  g_list_free (keys);
  g_object_unref (options);
  g_object_unref (audio);
  return NULL;
}

static void
test_resolve_fingerprint (void)
{
  GrlSource *source;
  guint i;

  struct {
    gchar *fingerprint_file;
    gint   duration;
    gchar *mb_artist_id;
    gchar *artist;
    gchar *mb_album_id;
    gchar *album;
    gchar *mb_recording_id;
    gchar *title;
  } audios[] = {
   { FINGERPRINT_LUDOVICO_EI, 445,
     "fa34b363-79df-434f-a5b8-be4e6898543f", "Ludovico Einaudi",
     "39f2c673-1387-4272-9db9-5f19d48e47cb", "Divenire",
     "70088e7c-1c01-48cb-9103-ba8b500c68a4", "Primavera" },
   { FINGERPRINT_NORAH_JONES, 160,
     "985c709c-7771-4de3-9024-7bda29ebe3f9", "Norah Jones",
     "8e264957-2754-4888-bbc5-9e165cd01d09", "Chasing Pirates Remix EP",
     "4116ff10-92cb-43e9-b45d-ea2262b186de", "Chasing Pirates (original album version)" },
   { FINGERPRINT_TROMBONE_SH, 243,
     "cae4fd51-4d58-4d48-92c1-6198cc2e45ed", "Trombone Shorty",
     "c3418122-387b-4477-90cf-e5e6d110e054", "For True",
     "96483bdd-f219-4ae3-a94e-04feeeef22a4", "Buckjump" },
   { FINGERPRINT_PHILIP_GLAS, 601,
     "5ae54dee-4dba-49c0-802a-a3b3b3adfe9b", "Philip Glass",
     "52f1f9d5-5166-4ceb-9289-6fb1a87f367c", "The Passion of Ramakrishna",
     "298e15a1-b29b-4947-9dca-ec3634f9ebde", "Part 2" },
   { FINGERPRINT_RADIOHEAD_PA, 385,
     "a74b1b7f-71a5-4011-9441-d0b5e4122711", "Radiohead",
     "67b2ff2a-38e9-32d0-84a0-85ac98cb16b4", "The Greatest Hits of 1997",
     "33e369bb-b8de-402f-b88f-594dea3544dc", "Paranoid Android",
   },
  };

  source = test_lua_factory_get_source (ACOUSTID_ID, ACOUSTID_OPS);
  for (i = 0; i < G_N_ELEMENTS (audios); i++) {
    gchar *data;
    GFile *file;
    gsize size;
    GError *error = NULL;
    gchar *mb_artist_id, *artist, *mb_album_id, *album, *mb_recording_id, *title;

    file = g_file_new_for_uri (audios[i].fingerprint_file);
    g_file_load_contents (file, NULL, &data, &size, NULL, &error);
    g_assert_no_error (error);
    g_clear_pointer (&file, g_object_unref);

    resolve (source, data, audios[i].duration,
             &mb_artist_id, &artist, &mb_album_id, &album, &mb_recording_id, &title);
    g_free (data);

    g_assert_cmpstr (audios[i].title, ==, title);
    g_free (title);
    g_assert_cmpstr (audios[i].mb_artist_id , ==, mb_artist_id);
    g_free (mb_artist_id);
    g_assert_cmpstr (audios[i].artist, ==, artist);
    g_free (artist);
    g_assert_cmpstr (audios[i].mb_album_id, ==, mb_album_id);
    g_free (mb_album_id);
    g_assert_cmpstr (audios[i].album, ==, album);
    g_free (album);
    g_assert_cmpstr (audios[i].mb_recording_id, ==, mb_recording_id);
    g_free (mb_recording_id);
  }
}

static void
test_acoustid_setup (gint *p_argc,
                     gchar ***p_argv)
{
  GrlRegistry *registry;
  GrlConfig *config;
  GError *error = NULL;

  g_setenv ("GRL_PLUGIN_PATH", TEST_PLUGINS_PATH, TRUE);
  g_setenv ("GRL_PLUGIN_LIST", TEST_PLUGINS_LOAD, TRUE);
  g_setenv ("GRL_LUA_SOURCES_PATH", LUA_FACTORY_SOURCES_PATH, TRUE);
  g_setenv ("GRL_NET_MOCKED", LUA_FACTORY_SOURCES_DATA_PATH "config.ini", TRUE);

  grl_init (p_argc, p_argv);
  g_test_init (p_argc, p_argv, NULL);

  /* This test uses 'chromaprint' metadata-key which is created by
   * Chromaprint plugin. For that reason, we need to load and activate it. */
  registry = grl_registry_get_default ();
  grl_registry_load_plugin (registry, CHROMAPRINT_PLUGIN_PATH "/libgrlchromaprint.so", &error);
  g_assert_no_error (error);
  grl_registry_activate_plugin_by_id (registry, CHROMAPRINT_ID, &error);
  g_assert_no_error (error);

  config = grl_config_new (LUA_FACTORY_ID, ACOUSTID_ID);
  grl_config_set_api_key (config, "ACOUSTID_TEST_MOCK_API_KEY");
  test_lua_factory_setup (config);
}

gint
main (gint argc, gchar **argv)
{
  test_acoustid_setup (&argc, &argv);

  g_test_add_func ("/lua_factory/sources/acoustid", test_resolve_fingerprint);

  gint result = g_test_run ();

  test_lua_factory_shutdown ();
  test_lua_factory_deinit ();

  return result;
}
