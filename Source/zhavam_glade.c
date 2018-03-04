/*
 * zhavam_glade.c
 *
 *  Created on: 4 mar. 2018
 *      Author: ipserc
 */

/**
 * Glade GYK Window Source
 * Escape the quotes " -> \"
 * Terminate with NULL
 */
const char * gtkSource = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<!-- Generated with glade 3.20.0 -->\
<interface>\
  <requires lib=\"gtk+\" version=\"3.20\"/>\
  <object class=\"GtkMenu\">\
    <property name=\"visible\">True</property>\
    <property name=\"can_focus\">False</property>\
  </object>\
  <object class=\"GtkAdjustment\" id=\"acrCloudTimeOutAdjustment\">\
    <property name=\"upper\">50000</property>\
    <property name=\"step_increment\">1</property>\
    <property name=\"page_increment\">10</property>\
  </object>\
  <object class=\"GtkAdjustment\" id=\"alsaPcmBufferFramesAdjustment\">\
    <property name=\"upper\">999999999</property>\
    <property name=\"step_increment\">100</property>\
    <property name=\"page_increment\">1000</property>\
  </object>\
  <object class=\"GtkAdjustment\" id=\"alsaRateAdjustment\">\
    <property name=\"lower\">8000</property>\
    <property name=\"upper\">5000000</property>\
    <property name=\"step_increment\">10</property>\
    <property name=\"page_increment\">100</property>\
  </object>\
  <object class=\"GtkTextBuffer\" id=\"trackInfoTextBuffer\"/>\
  <object class=\"GtkImage\" id=\"zhavamImage\">\
    <property name=\"visible\">True</property>\
    <property name=\"can_focus\">False</property>\
    <property name=\"pixbuf\">Zhavam.01.svg</property>\
    <property name=\"pixel_size\">317</property>\
    <property name=\"use_fallback\">True</property>\
    <property name=\"icon_size\">6</property>\
  </object>\
  <object class=\"GtkApplicationWindow\" id=\"zhavamMainWindow\">\
    <property name=\"can_focus\">False</property>\
    <property name=\"window_position\">center</property>\
    <property name=\"default_width\">-1</property>\
    <property name=\"default_height\">-1</property>\
    <property name=\"icon\">Zhavam.01.svg.png</property>\
    <child>\
      <object class=\"GtkBox\" id=\"zhavamMainWindowBox\">\
        <property name=\"visible\">True</property>\
        <property name=\"can_focus\">False</property>\
        <property name=\"orientation\">vertical</property>\
        <child>\
          <object class=\"GtkMenuBar\" id=\"zhavamMWMenuBar\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <child>\
              <object class=\"GtkMenuItem\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">_Config</property>\
                <property name=\"use_underline\">True</property>\
                <child type=\"submenu\">\
                  <object class=\"GtkMenu\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuConfigEdit\">\
                        <property name=\"label\">gtk-edit</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuConfigReload\">\
                        <property name=\"label\">gtk-revert-to-saved</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuConfigSave\">\
                        <property name=\"label\">gtk-save</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                    <child>\
                      <object class=\"GtkSeparatorMenuItem\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                      </object>\
                    </child>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuConfigQuit\">\
                        <property name=\"label\">gtk-quit</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child>\
              <object class=\"GtkMenuItem\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">_Info</property>\
                <property name=\"use_underline\">True</property>\
                <child type=\"submenu\">\
                  <object class=\"GtkMenu\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuInfoCopy\">\
                        <property name=\"label\">gtk-copy</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuInfoClearText\">\
                        <property name=\"label\">gtk-clear</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child>\
              <object class=\"GtkMenuItem\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">Ay_uda</property>\
                <property name=\"use_underline\">True</property>\
                <child type=\"submenu\">\
                  <object class=\"GtkMenu\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkImageMenuItem\" id=\"menuHelpAbout\">\
                        <property name=\"label\">gtk-about</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"use_underline\">True</property>\
                        <property name=\"use_stock\">True</property>\
                        <property name=\"always_show_image\">True</property>\
                      </object>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">True</property>\
            <property name=\"position\">0</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkFrame\" id=\"devicesFrame\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <property name=\"margin_left\">3</property>\
            <property name=\"margin_right\">3</property>\
            <property name=\"margin_top\">3</property>\
            <property name=\"margin_bottom\">3</property>\
            <property name=\"label_xalign\">0</property>\
            <child>\
              <object class=\"GtkAlignment\" id=\"devicesAlignment\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"top_padding\">3</property>\
                <property name=\"bottom_padding\">3</property>\
                <property name=\"left_padding\">12</property>\
                <child>\
                  <object class=\"GtkBox\" id=\"devicesBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"devicesLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">6</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Rec. Device</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkComboBoxText\" id=\"devicesComboBoxText\">\
                        <property name=\"width_request\">0</property>\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child type=\"label\">\
              <object class=\"GtkLabel\" id=\"devicesLabelFrame\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">Devices</property>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">True</property>\
            <property name=\"position\">1</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkBox\" id=\"recordBox\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <property name=\"margin_left\">3</property>\
            <property name=\"margin_right\">3</property>\
            <property name=\"margin_top\">3</property>\
            <property name=\"margin_bottom\">3</property>\
            <property name=\"homogeneous\">True</property>\
            <child>\
              <object class=\"GtkToggleButton\" id=\"recordToggleButton\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">True</property>\
                <property name=\"receives_default\">True</property>\
                <property name=\"image\">zhavamImage</property>\
                <property name=\"always_show_image\">True</property>\
                <property name=\"draw_indicator\">True</property>\
              </object>\
              <packing>\
                <property name=\"expand\">False</property>\
                <property name=\"fill\">True</property>\
                <property name=\"position\">1</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">False</property>\
            <property name=\"position\">2</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkFrame\" id=\"trackInfoFrame\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <property name=\"margin_left\">3</property>\
            <property name=\"margin_right\">3</property>\
            <property name=\"margin_top\">3</property>\
            <property name=\"margin_bottom\">3</property>\
            <property name=\"label_xalign\">0</property>\
            <property name=\"shadow_type\">etched-out</property>\
            <child>\
              <object class=\"GtkAlignment\" id=\"trackInfoAlignment\">\
                <property name=\"width_request\">50</property>\
                <property name=\"height_request\">130</property>\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"margin_left\">3</property>\
                <property name=\"margin_right\">3</property>\
                <property name=\"margin_top\">3</property>\
                <property name=\"margin_bottom\">3</property>\
                <property name=\"top_padding\">3</property>\
                <property name=\"bottom_padding\">3</property>\
                <property name=\"left_padding\">3</property>\
                <property name=\"right_padding\">3</property>\
                <child>\
                  <object class=\"GtkScrolledWindow\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">True</property>\
                    <property name=\"window_placement\">bottom-left</property>\
                    <property name=\"shadow_type\">in</property>\
                    <property name=\"propagate_natural_width\">True</property>\
                    <property name=\"propagate_natural_height\">True</property>\
                    <child>\
                      <object class=\"GtkTextView\" id=\"trackInfoGtkTextView\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"editable\">False</property>\
                        <property name=\"wrap_mode\">word</property>\
                        <property name=\"cursor_visible\">False</property>\
                        <property name=\"buffer\">trackInfoTextBuffer</property>\
                        <property name=\"accepts_tab\">False</property>\
                      </object>\
                    </child>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child type=\"label\">\
              <object class=\"GtkLabel\" id=\"trackInfoLabelFrame\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">Track Info</property>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">True</property>\
            <property name=\"fill\">True</property>\
            <property name=\"position\">3</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkFrame\" id=\"statusFrame\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <property name=\"margin_top\">3</property>\
            <property name=\"margin_bottom\">3</property>\
            <property name=\"label_xalign\">0</property>\
            <property name=\"shadow_type\">etched-out</property>\
            <child>\
              <object class=\"GtkAlignment\" id=\"statusAlignment\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"top_padding\">3</property>\
                <property name=\"bottom_padding\">3</property>\
                <property name=\"left_padding\">12</property>\
                <child>\
                  <object class=\"GtkLabel\" id=\"statusZhvLabel\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <property name=\"halign\">end</property>\
                    <property name=\"margin_top\">3</property>\
                    <property name=\"margin_bottom\">3</property>\
                    <property name=\"label\" translatable=\"yes\">Status label</property>\
                    <property name=\"justify\">right</property>\
                    <property name=\"wrap\">True</property>\
                    <property name=\"ellipsize\">end</property>\
                    <property name=\"lines\">1</property>\
                  </object>\
                </child>\
              </object>\
            </child>\
            <child type=\"label\">\
              <object class=\"GtkLabel\" id=\"statusLabelFrame\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">Status</property>\
              </object>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">True</property>\
            <property name=\"pack_type\">end</property>\
            <property name=\"position\">4</property>\
          </packing>\
        </child>\
      </object>\
    </child>\
  </object>\
  <object class=\"GtkAboutDialog\" id=\"aboutZhavam\">\
    <property name=\"can_focus\">False</property>\
    <property name=\"modal\">True</property>\
    <property name=\"window_position\">center</property>\
    <property name=\"destroy_with_parent\">True</property>\
    <property name=\"type_hint\">dialog</property>\
    <property name=\"transient_for\">zhavamMainWindow</property>\
    <property name=\"attached_to\">menuHelpAbout</property>\
    <property name=\"program_name\">Zhavam (powered by Gtk+ 3.12)</property>\
    <property name=\"version\">1.0</property>\
    <property name=\"comments\" translatable=\"yes\">Zhavam helps you to identify the music or song that is beign reproduced from or outside of your computer. You can also try singing or whistle the song you are looking for, but  you need have got a micro connected to it.</property>\
    <property name=\"website\">http://selenitas.es/zhavam</property>\
    <property name=\"website_label\" translatable=\"yes\">Zhavam</property>\
    <property name=\"license\" translatable=\"yes\">Este programa viene SIN NINGUNA GARANTÍA.\
Consulte la &lt;a href=\"https://www.gnu.org/licenses/gpl-3.0.html\"&gt;Licencia Pública General de GNU, versión 3 o posterior&lt;/a&gt; para obtener más detalles.</property>\
    <property name=\"authors\">ipserc (aka Jose Luis)\
 \
Zhavam was born because of I was looking for a Shazam-like\
application for Linux, but after several tries in internert I couldn't find \
anything that gives me the basic functionallity, it is, sound recognition.\
\
After looking for something related with music recognition in the web \
I found ACRCloud.\
\
ACRCloud gives you the services required for accessing thier database\
and ask it about a piece of a song. If you are lucky, the ACRCloud service \
recognizes your song piece from the millions kept in the database, and it \
reply to your request by giving you some data about of the song you sent.\
\
Zhavam connects with Alsa to record a small piece of the song, and then \
sends it to ACRCloud. ACRCloud responds with a Json message with the \
information about the Title of the song, the artist, the album, the genre\
the year, etc. zhavam parses this message putting the most relevant \
information in the Track Info text view.\
\
For using zhavam in your computer you have to have an account in ACRCloud. \
Registering in ACRCloud brings you the host to access through the \
acces key and access secret required to start a session with teh server.\
\
Before using zhavam, please register yourself in ACRCloud and put the host, \
access key and access secret into the config file of zhavam.</property>\
    <property name=\"logo\">Zhavam.01.svg</property>\
    <property name=\"wrap_license\">True</property>\
    <property name=\"license_type\">gpl-3-0</property>\
    <signal name=\"close\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <signal name=\"delete-event\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <signal name=\"destroy\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <child internal-child=\"vbox\">\
      <object class=\"GtkBox\">\
        <property name=\"visible\">True</property>\
        <property name=\"can_focus\">False</property>\
        <property name=\"orientation\">vertical</property>\
        <property name=\"spacing\">2</property>\
        <child internal-child=\"action_area\">\
          <object class=\"GtkButtonBox\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <property name=\"layout_style\">start</property>\
            <child>\
              <placeholder/>\
            </child>\
            <child>\
              <placeholder/>\
            </child>\
            <child>\
              <placeholder/>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">False</property>\
            <property name=\"pack_type\">end</property>\
            <property name=\"position\">0</property>\
          </packing>\
        </child>\
        <child>\
          <placeholder/>\
        </child>\
      </object>\
    </child>\
  </object>\
  <object class=\"GtkDialog\" id=\"configDialog\">\
    <property name=\"can_focus\">False</property>\
    <property name=\"modal\">True</property>\
    <property name=\"window_position\">center-on-parent</property>\
    <property name=\"destroy_with_parent\">True</property>\
    <property name=\"type_hint\">dialog</property>\
    <property name=\"transient_for\">zhavamMainWindow</property>\
    <property name=\"attached_to\">zhavamMainWindow</property>\
    <signal name=\"close\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <signal name=\"delete-event\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <child internal-child=\"vbox\">\
      <object class=\"GtkBox\">\
        <property name=\"can_focus\">False</property>\
        <property name=\"orientation\">vertical</property>\
        <property name=\"spacing\">2</property>\
        <child internal-child=\"action_area\">\
          <object class=\"GtkButtonBox\" id=\"configButtonBox\">\
            <property name=\"can_focus\">False</property>\
            <property name=\"layout_style\">end</property>\
            <child>\
              <object class=\"GtkButton\" id=\"configCleanButton\">\
                <property name=\"label\">gtk-clear</property>\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">True</property>\
                <property name=\"receives_default\">True</property>\
                <property name=\"use_stock\">True</property>\
                <property name=\"always_show_image\">True</property>\
              </object>\
              <packing>\
                <property name=\"expand\">True</property>\
                <property name=\"fill\">True</property>\
                <property name=\"position\">1</property>\
              </packing>\
            </child>\
            <child>\
              <object class=\"GtkButton\" id=\"configSaveButton\">\
                <property name=\"label\">gtk-save</property>\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">True</property>\
                <property name=\"receives_default\">True</property>\
                <property name=\"use_stock\">True</property>\
                <property name=\"always_show_image\">True</property>\
              </object>\
              <packing>\
                <property name=\"expand\">True</property>\
                <property name=\"fill\">True</property>\
                <property name=\"position\">2</property>\
              </packing>\
            </child>\
            <child>\
              <object class=\"GtkButton\" id=\"configCloseButton\">\
                <property name=\"label\">gtk-close</property>\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">True</property>\
                <property name=\"receives_default\">True</property>\
                <property name=\"use_stock\">True</property>\
                <property name=\"always_show_image\">True</property>\
              </object>\
              <packing>\
                <property name=\"expand\">True</property>\
                <property name=\"fill\">True</property>\
                <property name=\"position\">3</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">False</property>\
            <property name=\"position\">0</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkNotebook\" id=\"configNoteBook\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">True</property>\
            <property name=\"group_name\">acrcloud</property>\
            <child>\
              <object class=\"GtkBox\" id=\"acrCloudBox\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"orientation\">vertical</property>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acrCloudHostLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"halign\">start</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Server Hostname</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkEntry\" id=\"acrCloudHostEntry\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acrCloudAccessKeyLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Access Key</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkEntry\" id=\"acrCloudAccessKeyEntry\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acrCloudAccessSecretLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Access Secret</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkEntry\" id=\"acrCloudAccessSecretEntry\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acrCloudTimeOutLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Time Out</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkSpinButton\" id=\"acrCloudTimeOutSpinButton\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"progress_pulse_step\">0</property>\
                        <property name=\"input_purpose\">number</property>\
                        <property name=\"adjustment\">acrCloudTimeOutAdjustment</property>\
                        <property name=\"climb_rate\">10</property>\
                        <property name=\"numeric\">True</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">3</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"acrCloudRecTypeLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Recording Type</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkComboBoxText\" id=\"acrCloudRecTypeComboBoxText\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">4</property>\
                  </packing>\
                </child>\
              </object>\
            </child>\
            <child type=\"tab\">\
              <object class=\"GtkLabel\" id=\"acrCloudNoteBookLabel\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">acrCloud</property>\
              </object>\
              <packing>\
                <property name=\"tab_fill\">False</property>\
              </packing>\
            </child>\
            <child>\
              <object class=\"GtkBox\" id=\"alsaBox\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"orientation\">vertical</property>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"alsaSndPcmFormatLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Snd PCM Format</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkComboBoxText\" id=\"alsaSndPcmFormatComboBoxText\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">0</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"alsaRateLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">Recording Rate</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkSpinButton\" id=\"alsaRateSpinButton\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"input_purpose\">number</property>\
                        <property name=\"adjustment\">alsaRateAdjustment</property>\
                        <property name=\"climb_rate\">10</property>\
                        <property name=\"numeric\">True</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">1</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"alsaPcmBufferFramesLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">PCM Buffer Frames</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkSpinButton\" id=\"alsaPcmBufferFramesSpinButton\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">True</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"input_purpose\">number</property>\
                        <property name=\"adjustment\">alsaPcmBufferFramesAdjustment</property>\
                        <property name=\"climb_rate\">10</property>\
                        <property name=\"numeric\">True</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"pack_type\">end</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">2</property>\
                  </packing>\
                </child>\
                <child>\
                  <object class=\"GtkBox\">\
                    <property name=\"visible\">True</property>\
                    <property name=\"can_focus\">False</property>\
                    <child>\
                      <object class=\"GtkLabel\" id=\"alsaPcmDeviceLabel\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                        <property name=\"label\" translatable=\"yes\">PCM Device</property>\
                        <property name=\"width_chars\">17</property>\
                        <property name=\"max_width_chars\">20</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">False</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">0</property>\
                      </packing>\
                    </child>\
                    <child>\
                      <object class=\"GtkComboBoxText\" id=\"alsaPcmDeviceComboBoxText\">\
                        <property name=\"visible\">True</property>\
                        <property name=\"can_focus\">False</property>\
                        <property name=\"margin_left\">3</property>\
                        <property name=\"margin_right\">3</property>\
                        <property name=\"margin_top\">3</property>\
                        <property name=\"margin_bottom\">3</property>\
                      </object>\
                      <packing>\
                        <property name=\"expand\">True</property>\
                        <property name=\"fill\">True</property>\
                        <property name=\"position\">1</property>\
                      </packing>\
                    </child>\
                  </object>\
                  <packing>\
                    <property name=\"expand\">True</property>\
                    <property name=\"fill\">False</property>\
                    <property name=\"position\">3</property>\
                  </packing>\
                </child>\
              </object>\
              <packing>\
                <property name=\"position\">1</property>\
              </packing>\
            </child>\
            <child type=\"tab\">\
              <object class=\"GtkLabel\" id=\"alsanoteBookLabel\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"label\" translatable=\"yes\">alsa</property>\
              </object>\
              <packing>\
                <property name=\"position\">1</property>\
                <property name=\"tab_fill\">False</property>\
              </packing>\
            </child>\
            <child>\
              <placeholder/>\
            </child>\
            <child type=\"tab\">\
              <placeholder/>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">True</property>\
            <property name=\"fill\">True</property>\
            <property name=\"position\">1</property>\
          </packing>\
        </child>\
      </object>\
    </child>\
  </object>\
  <object class=\"GtkDialog\" id=\"dialogWarning\">\
    <property name=\"can_focus\">False</property>\
    <property name=\"valign\">start</property>\
    <property name=\"title\" translatable=\"yes\">Warning</property>\
    <property name=\"resizable\">False</property>\
    <property name=\"modal\">True</property>\
    <property name=\"window_position\">center-on-parent</property>\
    <property name=\"destroy_with_parent\">True</property>\
    <property name=\"icon\">50697_121100_warning.png</property>\
    <property name=\"type_hint\">dialog</property>\
    <property name=\"transient_for\">zhavamMainWindow</property>\
    <property name=\"attached_to\">zhavamMainWindow</property>\
    <signal name=\"close\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <signal name=\"destroy\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <signal name=\"destroy-event\" handler=\"gtk_widget_hide\" swapped=\"no\"/>\
    <child internal-child=\"vbox\">\
      <object class=\"GtkBox\">\
        <property name=\"can_focus\">False</property>\
        <property name=\"halign\">center</property>\
        <property name=\"valign\">center</property>\
        <property name=\"orientation\">vertical</property>\
        <property name=\"spacing\">2</property>\
        <child internal-child=\"action_area\">\
          <object class=\"GtkButtonBox\">\
            <property name=\"can_focus\">False</property>\
            <property name=\"halign\">end</property>\
            <property name=\"valign\">end</property>\
            <property name=\"homogeneous\">True</property>\
            <property name=\"layout_style\">end</property>\
            <child>\
              <object class=\"GtkButton\" id=\"buttonDialogWarning\">\
                <property name=\"label\">gtk-ok</property>\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">True</property>\
                <property name=\"focus_on_click\">False</property>\
                <property name=\"can_default\">True</property>\
                <property name=\"has_default\">True</property>\
                <property name=\"receives_default\">True</property>\
                <property name=\"halign\">end</property>\
                <property name=\"valign\">end</property>\
                <property name=\"margin_left\">3</property>\
                <property name=\"margin_right\">3</property>\
                <property name=\"margin_top\">3</property>\
                <property name=\"margin_bottom\">3</property>\
                <property name=\"use_stock\">True</property>\
              </object>\
              <packing>\
                <property name=\"expand\">False</property>\
                <property name=\"fill\">False</property>\
                <property name=\"position\">-1</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">True</property>\
            <property name=\"fill\">True</property>\
            <property name=\"pack_type\">end</property>\
            <property name=\"position\">0</property>\
          </packing>\
        </child>\
        <child>\
          <object class=\"GtkBox\" id=\"box1\">\
            <property name=\"visible\">True</property>\
            <property name=\"can_focus\">False</property>\
            <child>\
              <object class=\"GtkImage\" id=\"imageDialogWarning\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"margin_left\">3</property>\
                <property name=\"margin_right\">3</property>\
                <property name=\"margin_top\">3</property>\
                <property name=\"margin_bottom\">3</property>\
                <property name=\"pixbuf\">50697_121100_warning.png</property>\
                <property name=\"icon_size\">6</property>\
              </object>\
              <packing>\
                <property name=\"expand\">True</property>\
                <property name=\"fill\">True</property>\
                <property name=\"position\">0</property>\
              </packing>\
            </child>\
            <child>\
              <object class=\"GtkLabel\" id=\"labelDialogWarning\">\
                <property name=\"visible\">True</property>\
                <property name=\"can_focus\">False</property>\
                <property name=\"halign\">start</property>\
                <property name=\"valign\">end</property>\
                <property name=\"margin_left\">3</property>\
                <property name=\"margin_right\">3</property>\
                <property name=\"margin_top\">3</property>\
                <property name=\"margin_bottom\">3</property>\
                <property name=\"label\" translatable=\"yes\">Aviso Warning Achtung</property>\
                <property name=\"wrap\">True</property>\
                <property name=\"max_width_chars\">80</property>\
                <property name=\"lines\">4</property>\
              </object>\
              <packing>\
                <property name=\"expand\">False</property>\
                <property name=\"fill\">False</property>\
                <property name=\"position\">1</property>\
              </packing>\
            </child>\
          </object>\
          <packing>\
            <property name=\"expand\">False</property>\
            <property name=\"fill\">False</property>\
            <property name=\"position\">1</property>\
          </packing>\
        </child>\
      </object>\
    </child>\
  </object>\
</interface>\
\0";

char ** getGtkSource(void)
{
	return gtkSource;
}


