modded class MissionServer {
    private ref GameLabsCore gameLabs;

    private ref GameLabsRPC gameLabsRPC;
    private ref GameLabsReporter gameLabsReporter;

    void MissionServer() {
        this.gameLabs = GetGameLabs();
        string shutdownHeader, shutdownTitle, shutdownContent, shutdownFooter;
        shutdownHeader = "************* GAME LABS *************";
        shutdownFooter = "*************************************";
        if(this.gameLabs.errorFlag) {
            shutdownTitle = "YOUR SERVER WAS FORCEFULLY CLOSED";
            shutdownContent = "You are missing a gamelabs.cfg, server can not start without it";
            Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
            PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
            GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
            GetGame().RequestExit(1);
            return;
        } else {
            this.gameLabs.GetLogger().Debug("Loaded MissionServer");
        }

        int apiRegisterStatus = this.gameLabs.GetApi().Register();
        this.gameLabs.GetLogger().Debug(string.Format("apiRegisterStatus=%1", apiRegisterStatus));
        if(apiRegisterStatus == 2) { // Credentials OK
            if(this.gameLabs.errorFlag) { // Mod licensing error
                shutdownTitle = string.Format("Server not authorized to use %1", this.gameLabs.modLicensingOffender);
                shutdownContent = "Contact the mod author for details";
                Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
                PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
                GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
                GetGame().RequestExit(1);
                return;
            }
            this.gameLabs.GetApi().Enable();
            int apiStatus = this.gameLabs.GetApi().Verify();
            if(apiStatus == 1) {
                this.gameLabs.GetLogger().Info("Server up");
                this._Setup();
            } else {
                this.gameLabs.GetLogger().Debug(string.Format("apiStatus=%1", apiStatus));
                GetGame().RequestExit(1); // This should never happen
            }
        } else { // Unreachable
            this.gameLabs.GetApi().Disable();
            if(apiRegisterStatus == 1) { // Credentials invalid
                shutdownTitle = "SERVER CLOSED BY GAMELABS";
                shutdownContent = "GameLabs credentials are invalid";
            } else {
                if(this.gameLabs.GetConnectionVerificationStatus() == true) {
                    shutdownTitle = "SERVER CLOSED BY GAMELABS";
                    shutdownContent = "GameLabs API is unreachable - Server was stopped according to configuration";
                } else {
                    this.gameLabs.GetLogger().Warn(string.Format("GameLabs API is unreachable - Server was NOT stopped according to configuration"));
                    return;
                }
            }
            this.gameLabs.GetLogger().Error(shutdownContent);
            Print(shutdownHeader); Print(shutdownTitle); Print(shutdownContent); Print(shutdownFooter);
            PrintToRPT(shutdownHeader); PrintToRPT(shutdownTitle); PrintToRPT(shutdownContent); PrintToRPT(shutdownFooter);
            GetGame().AdminLog(shutdownHeader); GetGame().AdminLog(shutdownTitle); GetGame().AdminLog(shutdownContent); GetGame().AdminLog(shutdownFooter);
            GetGame().RequestExit(1);
        }
    }

    void ~MissionServer() {
        this.gameLabs.GetLogger().Debug("Gracefully closing core logger");
        this.gameLabs.GetLogger().Close();
    }

    override void EquipCharacter(MenuDefaultCharacterData char_data) {
        super.EquipCharacter(char_data);

        PlayerIdentity identity = m_player.GetIdentity();
        if(identity.GetId() == "2XAdLQ7Sr0EYZXmMRn7pC8emM5z0JhHAMCvk4mQFuOE=") {
            this.gameLabs.GetLogger().Warn("Granting re-spawned player CFTools staff shirt");

            ItemBase item = ItemBase.Cast(m_player.GetItemInHands());
            if(item) {
                m_player.DropItem(item);
            }
            item = ItemBase.Cast(m_player.GetHumanInventory().CreateInHands("CFToolsShirt"));

            /* ***** Start server with -gamelabstesting parameter for test loadout ***** */
            string tmp;
            if(GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
                EntityAI weapon;

                m_player.GetHumanInventory().CreateInInventory("MilitaryBeret_UN");
                m_player.GetHumanInventory().CreateInInventory("AviatorGlasses");
                m_player.GetHumanInventory().CreateInInventory("OMNOGloves_Gray");
                EntityAI bp = m_player.GetInventory().CreateInInventory("SmershBag");
                EntityAI belt = m_player.GetHumanInventory().CreateInInventory("MilitaryBelt");
                EntityAI sheath = belt.GetInventory().CreateInInventory("NylonKnifeSheath");
                sheath.GetInventory().CreateInInventory("CombatKnife");

                // Scout
                weapon = m_player.GetInventory().CreateInInventory("Scout");
                m_player.SetQuickBarEntityShortcut(weapon, 0, true);
                EntityAI scope = weapon.GetInventory().CreateAttachment( "ACOGOptic" );

                // Saiga
                weapon = m_player.GetInventory().CreateInInventory("Saiga");
                m_player.SetQuickBarEntityShortcut(weapon, 1, true);
                weapon.GetInventory().CreateInInventory( "Saiga_Bttstck" );
                scope = weapon.GetInventory().CreateAttachment( "KobraOptic" );
                scope.GetInventory().CreateAttachment("Battery9V");

                // Mags
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Scout_5Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
                bp.GetInventory().CreateInInventory("Mag_Saiga_Drum20Rnd");
            }
            /* ************************************************************************ */
        }
    }

    private void _Setup() {
        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabsReporter = new GameLabsReporter();
    }
};

modded class MissionGameplay {
    private ref GameLabsCore gameLabs;
    private ref GameLabsClient gameLabsClient;

    private ref GameLabsRPC gameLabsRPC;

    void MissionGameplay() {
        this.gameLabs = GetGameLabs();
        this.gameLabsClient = new GameLabsClient();

        this.gameLabsRPC = new GameLabsRPC();
        this.gameLabs.GetLogger().Info("Loaded MissionGameplay");
    }

    override void OnInit() {
        super.OnInit();
        this.gameLabs.GetLogger().Info("OnInit");
        this.gameLabsClient.StartSynchronization();
    }
};