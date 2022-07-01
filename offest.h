bool UpdateAddress() {

	Uworld = READ64(m_base + 0xcbf38b8);
	if ((Uworld == 0)) return false;
	Gameinstance = READ64(Uworld + 0x170);
	if ((Gameinstance == 0)) return false;
	LocalPlayers = READ64(Gameinstance + 0x38);
	if ((LocalPlayers == 0)) return false;
	Localplayer = READ64(LocalPlayers);
	if ((Localplayer == 0)) return false;
	PlayerController = READ64(LocalPlayers + 0x30);
	if ((PlayerController == 0)) return false;
	LocalPawn = READ64(PlayerController + 0x308);
	if ((LocalPawn == 0)) return false;
	Rootcomp = READ64(LocalPawn + 0x130);
	if ((Rootcomp == 0)) return false;
	Ulevel = READ64(Uworld + 0x30);
	if ((Ulevel == 0)) return false;
	ActorCount = READ64(Ulevel + 0xA0);
	if ((ActorCount == 0)) return false;
	AActors = READ64(Ulevel + 0x92);
	if ((AActors == 0)) return false;

	if (LocalPawn != 0) {
		localplayerID = ReadInteger(LocalPawn + 0xb8);
	}
	return true;
}
