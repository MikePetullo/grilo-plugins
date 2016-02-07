/*
 * Copyright (C) 2014 Igalia S.L.
 *
 * Author: Juan A. Suarez Romero <jasuarez@igalia.com>
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

#include <grilo.h>

#define GAMEFAQS_ID "grl-gamefaqs"

static GrlMedia *
build_game_media (const gchar *title)
{
  GrlMedia *media;

  media = grl_media_new ();
  grl_media_set_title (media, title);

  return media;
}

static void
test_setup (void)
{
  GError *error = NULL;
  GrlRegistry *registry;

  registry = grl_registry_get_default ();
  grl_registry_load_all_plugins (registry, TRUE, &error);
  g_assert_no_error (error);
}
#define DESCRIPTION "You're Strider, a crunching hulk of muscle with a grappling hook and laser sword. You're in a future land of backward time, and its evil Master is taunting you with defeat. Can you survive 8 megs of incredible danger? Hang-glide into the citadel, then clang your hook into metal to stiff-arm up to safety. Stay alive by slashing armies, winged robots, and prehistoric monsters into shuddering heaps! Battle alluring Amazons who sling vicious boomerangs and drum out a war chant to turn your mind manic. Slide, duck, and cartwheel past lasers, spikes and bombs. Screech to a halt in the face of giant metal monkeys and centipedes. Feel your biceps burst as you grapple with snarling musclemen. Cling to battleship bulkheads. Leap from Tyrannosaurus Rex. It's Strider! The Master awaits!"

static void
test_resolve_good_found (void)
{
  GError *error = NULL;
  GList *keys;
  GrlMedia *media;
  GrlOperationOptions *options;
  GrlRegistry *registry;
  GrlSource *source;
  GDateTime *date_time;
  guint expected_n_thumbnails, expected_n_external_urls;

  registry = grl_registry_get_default ();
  source = grl_registry_lookup_source (registry, GAMEFAQS_ID);
  g_assert (source);

  media = build_game_media ("Strider");
  grl_media_set_mime (media, "application/x-genesis-rom");

  keys = grl_metadata_key_list_new (GRL_METADATA_KEY_THUMBNAIL,
                                    GRL_METADATA_KEY_DESCRIPTION,
                                    GRL_METADATA_KEY_EXTERNAL_URL,
                                    GRL_METADATA_KEY_RATING,
                                    GRL_METADATA_KEY_PUBLICATION_DATE,
                                    GRL_METADATA_KEY_ORIGINAL_TITLE,
                                    NULL);

  options = grl_operation_options_new (NULL);
  grl_operation_options_set_resolution_flags (options, GRL_RESOLVE_FULL);

  grl_source_resolve_sync (source, media, keys, options, &error);

  g_assert_no_error (error);

  /* We should get a thumbnail */
  expected_n_thumbnails = grl_data_length (GRL_DATA (media), GRL_METADATA_KEY_THUMBNAIL);
  g_assert_cmpuint (expected_n_thumbnails, ==, 4);
  g_assert_cmpstr (grl_media_get_thumbnail_nth (media, 0),
                   ==,
                   "http://img.gamefaqs.net/box/0/9/7/297097_front.jpg");

  g_assert_cmpstr (grl_media_get_description (media),
                   ==,
                   DESCRIPTION);

  expected_n_external_urls = grl_data_length (GRL_DATA (media), GRL_METADATA_KEY_EXTERNAL_URL);
  g_assert_cmpuint (expected_n_external_urls, ==, 1);
  g_assert_cmpstr (grl_media_get_external_url (media),
                   ==,
                   "http://www.gamefaqs.com/genesis/586497-strider");

  /* Comparing floats fails with: (3.71000004 == 3.71) */
  g_assert_cmpint (grl_media_get_rating (media) * 100, ==, 371);

  date_time = grl_media_get_publication_date (media);
  g_assert_cmpint (g_date_time_get_year (date_time), ==, 1990);

  g_assert_cmpstr (grl_media_get_original_title (media),
                   ==,
                   "Strider Hiryu (JP)");

  g_list_free (keys);
  g_object_unref (options);
  g_object_unref (media);
}

static void
test_resolve_thumbnail_found (void)
{
  GError *error = NULL;
  GList *keys;
  GrlMedia *media;
  GrlOperationOptions *options;
  GrlRegistry *registry;
  GrlSource *source;
  guint expected_n_thumbnails;

  registry = grl_registry_get_default ();
  source = grl_registry_lookup_source (registry, GAMEFAQS_ID);
  g_assert (source);

  media = build_game_media ("Kirby & the Amazing Mirror");
  grl_media_set_mime (media, "application/x-gba-rom");

  keys = grl_metadata_key_list_new (GRL_METADATA_KEY_THUMBNAIL,
                                    NULL);

  options = grl_operation_options_new (NULL);
  grl_operation_options_set_resolution_flags (options, GRL_RESOLVE_FULL);

  grl_source_resolve_sync (source, media, keys, options, &error);

  g_assert_no_error (error);

  /* We should get a thumbnail */
  expected_n_thumbnails = grl_data_length (GRL_DATA (media), GRL_METADATA_KEY_THUMBNAIL);
  g_assert_cmpuint (expected_n_thumbnails, ==, 3);
  g_assert_cmpstr (grl_media_get_thumbnail_nth (media, 2),
                   ==,
                   "http://img.gamefaqs.net/box/9/8/5/57985_front.jpg");

  g_list_free (keys);
  g_object_unref (options);
  g_object_unref (media);
}

int
main(int argc, char **argv)
{
  g_setenv ("GRL_PLUGIN_PATH", LUA_FACTORY_PLUGIN_PATH, TRUE);
  g_setenv ("GRL_LUA_SOURCES_PATH", LUA_SOURCES_PATH, TRUE);
  g_setenv ("GRL_PLUGIN_LIST", "grl-lua-factory", TRUE);
  g_setenv ("GRL_NET_MOCKED", GAMEFAQS_DATA_PATH "network-data.ini", TRUE);

  grl_init (&argc, &argv);
  g_test_init (&argc, &argv, NULL);

  test_setup ();

  g_test_add_func ("/gamefaqs/resolve/good-found", test_resolve_good_found);
  g_test_add_func ("/gamefaqs/resolve/thumbnail-found", test_resolve_thumbnail_found);

  gint result = g_test_run ();

  grl_deinit ();

  return result;
}