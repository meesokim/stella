//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2017 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "CartEF.hxx"
#include "PopUpWidget.hxx"
#include "CartEFWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeEFWidget::CartridgeEFWidget(
      GuiObject* boss, const GUI::Font& lfont, const GUI::Font& nfont,
      int x, int y, int w, int h, CartridgeEF& cart)
  : CartDebugWidget(boss, lfont, nfont, x, y, w, h),
    myCart(cart)
{
  uInt32 size = 16 * 4096;

  ostringstream info;
  info << "64K H. Runner EF cartridge, 16 4K banks\n"
       << "Startup bank = " << cart.myStartBank << "\n";

  // Eventually, we should query this from the debugger/disassembler
  for(uInt32 i = 0, offset = 0xFFC, spot = 0xFE0; i < 16; ++i, offset += 0x1000)
  {
    uInt16 start = (cart.myImage[offset+1] << 8) | cart.myImage[offset];
    start -= start % 0x1000;
    info << "Bank " << std::dec << i << " @ $" << Common::Base::HEX4 << start << " - "
         << "$" << (start + 0xFFF) << " (hotspot = $" << (spot+i) << ")\n";
  }

  int xpos = 10,
      ypos = addBaseInformation(size, "Paul Slocum / Homestar Runner",
                                info.str()) + myLineHeight;

  VariantList items;
  VarList::push_back(items, " 0 ($FE0)");
  VarList::push_back(items, " 1 ($FE1)");
  VarList::push_back(items, " 2 ($FE2)");
  VarList::push_back(items, " 3 ($FE3)");
  VarList::push_back(items, " 4 ($FE4)");
  VarList::push_back(items, " 5 ($FE5)");
  VarList::push_back(items, " 6 ($FE6)");
  VarList::push_back(items, " 7 ($FE7)");
  VarList::push_back(items, " 8 ($FE8)");
  VarList::push_back(items, " 9 ($FE9)");
  VarList::push_back(items, "10 ($FEA)");
  VarList::push_back(items, "11 ($FEB)");
  VarList::push_back(items, "12 ($FEC)");
  VarList::push_back(items, "13 ($FED)");
  VarList::push_back(items, "14 ($FEE)");
  VarList::push_back(items, "15 ($FEF)");
  myBank =
    new PopUpWidget(boss, _font, xpos, ypos-2, _font.getStringWidth("15 ($FE0) "),
                    myLineHeight, items, "Set bank: ",
                    _font.getStringWidth("Set bank: "), kBankChanged);
  myBank->setTarget(this);
  addFocusWidget(myBank);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFWidget::loadConfig()
{
  myBank->setSelectedIndex(myCart.myCurrentBank);

  CartDebugWidget::loadConfig();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFWidget::handleCommand(CommandSender* sender,
                                      int cmd, int data, int id)
{
  if(cmd == kBankChanged)
  {
    myCart.unlockBank();
    myCart.bank(myBank->getSelected());
    myCart.lockBank();
    invalidate();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string CartridgeEFWidget::bankState()
{
  ostringstream& buf = buffer();

  static const char* spot[] = {
    "$FE0", "$FE1", "$FE2", "$FE3", "$FE4", "$FE5", "$FE6", "$FE7",
    "$FE8", "$FE9", "$FEA", "$FEB", "$FEC", "$FED", "$FEE", "$FEF"
  };
  buf << "Bank = " << std::dec << myCart.myCurrentBank
      << ", hotspot = " << spot[myCart.myCurrentBank];

  return buf.str();
}
