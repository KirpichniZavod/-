# ОТРАЖЕНИЕ — UE 5.4.4 (C++) минимальный каркас + UI

Готов базовый фреймворк: 1‑е лицо (ходьба/осмотр), взаимодействия (E) с подсказкой, пауза (Esc) с UMG, диалоги через DataAsset и виджет, GameInstance с сейв/лоад и загрузкой уровней.

## Быстрый старт
1) Создай C++ проект `Reflection` (UE 5.4.4). Закрой редактор. Скопируй `/Source/Reflection` из этой папки в проект.
2) Перегенерируй проектные файлы, собери.
3) Project Settings → Input: добавь биндинги (ниже).
4) Maps & Modes: выставь `GameMode` = `AReflectionGameMode` (он уже ставит `AReflectionCharacter` и `AReflectionPlayerController`).
5) Создай UMG:
   - `W_InteractPrompt` (интерфейс `UInteractPromptWidget`): реализуй `SetPromptText` (TextBlock RU: "Нажмите E, чтобы взаимодействовать").
   - `W_PauseMenu` (интерфейс `UPauseMenuWidget`): кнопки "Продолжить", "Выйти". Привяжи к `RequestResume`/`RequestQuit`.
   - `W_Dialogue` (интерфейс `UDialogueWidget`): реализуй `BP_ShowLine(Speaker, Text, Choices)` и `BP_OnDialogueClosed`.
6) В `AReflectionPlayerController` укажи классы этих виджетов (в BP-классе контроллера или Default value в C++ через BP).

## Ввод (Project Settings → Input)
Axis Mappings:
- MoveForward: W (+1), S (-1)
- MoveRight: D (+1), A (-1)
- Turn: Mouse X
- LookUp: Mouse Y

Action Mappings:
- Interact: E
- Menu: Escape

## Ключевые классы
- `AReflectionCharacter` — 1‑е лицо, трейс на ховере показывает подсказку, E вызывает `Interact` через интерфейс.
- `AReflectionPlayerController` — управляет UMG: подсказка, пауза, диалог.
- `UInteractable` + `AInteractableActor` — интерфейс и базовый актёр с `DisplayName` (RU).
- `UDialogueDataAsset` + `UDialogueWidget` + `UDialogueRunnerComponent` — данные и показ диалога (варианты).
- `UReflectionSaveGame`, `UReflectionGameInstance` — сейв/лоад и переход по уровням.
- `AReflectionGameMode` — подключает персонажа и контроллер.

## Как проверить
- Брось `AInteractableActor` на уровень, установи Static Mesh и `DisplayName` (например, "Кофеварка"). Наведи прицелом — увидишь подсказку RU. Нажми E — получишь debug.
- Нажми Esc — пауза (если указан класс `W_PauseMenu` в контроллере).
- Создай `DialogueDataAsset` с линиями — вызови `StartDialogue` из контроллера (Blueprint).

## RU реплики (готовые строки)
- Друг: "Тебя когда-нибудь пугала повторяемость? Как будто ты уже жил этот день."
- ГГ: "Кто ты вообще?"
- Друг: "А тебе не кажется, что никто здесь не смотрит?"
- Выборы: "Согласиться", "Уйти от разговора", "Промолчать"

## Замечания
- Скорость ходьбы: 300 uu/s. Бег/присед — отключены.
- Подсказка взаимодействия скрывается автоматически при уводе прицела.
- UMG внешний вид оформляется в редакторе.