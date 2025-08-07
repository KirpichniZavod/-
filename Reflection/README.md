# ОТРАЖЕНИЕ — UE 5.4.4 (C++) минимальный каркас

Этот набор исходников даёт основу под вертикалку: 1‑е лицо (ходьба/осмотр), взаимодействия (E), пауза (Esc), SaveGame заготовка и типы диалога. Контент (уровни, виджеты, кат‑сцены) собираются в редакторе.

## Быстрый старт
1) Создай в UE 5.4.4 C++ проект с именем `Reflection` (лучше с First Person шаблоном, но можно пустой).
2) Закрой редактор. В корне проекта перейди в `Source/Reflection` и замени содержимое на файлы из этой папки (`/Source/Reflection`).
3) Убедись, что `Reflection.Build.cs` на месте. Обнови проектные файлы (`.uproject` → правой кнопкой → Generate Visual Studio project files / или Xcode), собери.
4) Открой проект в UE, в Project Settings → Input добавь биндинги (см. ниже).
5) На любом уровне выставь GameMode по умолчанию на `AReflectionGameMode`. Он спавнит `AReflectionCharacter`.

## Ввод (Project Settings → Input)
Axis Mappings:
- MoveForward: W (+1), S (-1), Gamepad LeftY (+1/-1)
- MoveRight: D (+1), A (-1), Gamepad LeftX (+1/-1)
- Turn: Mouse X, Gamepad RightX
- LookUp: Mouse Y, Gamepad RightY

Action Mappings:
- Interact: E, Gamepad Face Button Bottom (A / Cross)
- Menu: Escape, Gamepad Special Right

## Состав кода
- `AReflectionCharacter` — персонаж от 1‑го лица: ходьба, взгляд, трейc взаимодействия по E, пауза по Esc.
- `UInteractable` — интерфейс для объектов взаимодействия (BlueprintNativeEvent).
- `AInteractableActor` — простой актёр с мешем, реализует интерфейс (показывает on-screen debug при взаимодействии).
- `UReflectionSaveGame` — SaveGame заготовка (sceneId, флаги).
- `UDialogueDataAsset` + `FDialogueLine/Choice` — типы для диалогов (заполняются в редакторе через DataAsset).
- `AReflectionGameMode` — ставит `AReflectionCharacter` как дефолтный Pawn.

Модули: Core, CoreUObject, Engine, InputCore, UMG, Slate, SlateCore.

## Использование
- Брось на уровень `AInteractableActor`, назначь Static Mesh, установи `DisplayName` (RU). Подойди и нажми E — увидишь debug‑сообщение.
- Для UI паузы создай UMG виджет `W_PauseMenu` и показывай его, подписавшись в Blueprints на событие паузы (функция `ToggleMenu` уже ставит GamePaused). В проде стоит добавить контроллер, который будет спаунить/прятать виджет.
- Диалоги: создай `DataAsset` класса `DialogueDataAsset` и заполни `Lines` (см. пример ниже). Показывать можно своим `W_Dialogue` (логика на Blueprints), читая `Lines` по индексу.

## Пример диалога (RU) для `UDialogueDataAsset`
Линия 0:
- Speaker: "Друг"
- Text: "Тебя когда-нибудь пугала повторяемость? Как будто ты уже жил этот день."
- Choices:
  - Text: "Ответить резко" → NextLineIndex: 1
  - Text: "Промолчать" → NextLineIndex: -1 (завершить)

Линия 1:
- Speaker: "ГГ"
- Text: "Кто ты вообще?"
- Choices: (пусто) → по завершению закрыть диалог

Дополнительно для офиса:
- 2: Друг — "А тебе не кажется, что никто здесь не смотрит?"
  - Выборы: "Согласиться" → 3, "Уйти от разговора" → -1
- 3: ГГ — "Может, ты прав..."

## Мини‑локализация (RU)
- Основное меню: Продолжить, Новая игра, Загрузить, Настройки, Выйти
- Пауза: Сохранить, Загрузить, Настройки, Выйти в меню
- Подсказка: "Нажмите E, чтобы взаимодействовать"

## Советы по контенту
- Камера — чистый 1‑е лицо без бега/приседа; скорость 300 uu/s.
- Взаимодействия — через интерфейс, сложную логику удобно делать в BP на базе `AInteractableActor`.
- Зацикливания/аномалии — таймлайны/Sequencer + пост‑процесс.

## Структура уровней (рекомендация)
- `L_010_Apartment_Morning` → `L_020_Stairs_Street` → `L_030_BusInterior` → `L_040_Office_Floor` → `L_050_Cafeteria` → `L_060_Street_Accident` → `L_070_Basement` → `L_080_Apartment_Variant`